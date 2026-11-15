// flight code for esp32-c3 supermini drone
// it listens to the esp-drone phone app over wifi (udp packets in crtp format)
]

#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiUdp.h>

// pins 
#define PIN_SDA 8
#define PIN_SCL 9
#define PIN_LED 10
#define PIN_BAT 2        // gpio2 is a strapping pin, i might have to move this

// motor pins in the order of the motor refs on my schematic (M1 to M4)
// the net labels on M3 and M4 are swapped, "Motor 4" goes to M3 and "Motor 3" goes to M4
const int motorPin[4] = {0, 1, 4, 3};

// wifi and app settings
const char* AP_SSID = "ESP-DRONE";
const char* AP_PASS = "12345678";
const uint16_t UDP_PORT = 2390;      // the app sends to this port

// pwm settings for the motors
#define PWM_FREQ 20000
#define PWM_BITS 10
#define PWM_MAX ((1 << PWM_BITS) - 1)

// loop timing and limits
#define LOOP_US 4000                 // 250hz
#define RAD2DEG 57.2957795f

const float MAX_THROTTLE = 0.95f;    // i keep some room at the top so the mixer can still add to the motors
const float MIN_THRUST   = 0.03f;    // under this i keep the motors off
const float IDLE_DUTY    = 0.06f;    // lowest duty while flying so the motors dont stall
const float MAX_ANGLE    = 30.0f;    // deg, the most tilt i let the app ask for
const float MAX_RATE     = 200.0f;   // deg/s
const float MAX_YAW_RATE = 150.0f;   // deg/s
const float TEST_DUTY    = 0.15f;    // duty for my serial motor test
const float LOW_BAT_MV   = 3300.0f;  // 1s lipo, i cut the motors under this

// pid gains, starting numbers only so i still have to tune these
const float ANGLE_KP = 4.0f;         // outer loop, deg error to wanted deg/s
const float RATE_KP  = 0.0015f;      // inner loop, the output is motor duty
const float RATE_KI  = 0.0020f;
const float RATE_KD  = 0.00002f;
const float YAW_KP   = 0.0030f;
const float YAW_KI   = 0.0020f;
const float I_LIMIT  = 0.10f;        // the most the integral can add to the motors

// i flip these if the drone leans the wrong way when i push the stick
const float CMD_ROLL_SIGN  = 1.0f;
const float CMD_PITCH_SIGN = 1.0f;
const float CMD_YAW_SIGN   = 1.0f;

// mixer for an x frame, seen from above with the front up
//   M1 front-left   M2 front-right
//   M3 rear-left    M4 rear-right
// M1 and M4 spin clockwise, M2 and M3 spin counter clockwise
// positive roll is left side up, positive pitch is nose down, positive yaw is counter clockwise
// if my motors sit or spin different i change the tables
const float ROLL_MIX[4]  = { 1, -1,  1, -1};
const float PITCH_MIX[4] = {-1, -1,  1,  1};
const float YAW_MIX[4]   = { 1, -1, -1,  1};

// mpu6050 registers
#define MPU_ADDR         0x68
#define REG_CONFIG       0x1A
#define REG_GYRO_CONFIG  0x1B
#define REG_ACCEL_CONFIG 0x1C
#define REG_ACCEL_XOUT_H 0x3B
#define REG_PWR_MGMT_1   0x6B
#define REG_WHO_AM_I     0x75

WiFiUDP udp;

// imu state
float gxOff = 0, gyOff = 0, gzOff = 0;               // gyro offsets from my calibration
float roll = 0, pitch = 0;                           // deg
float rollRate = 0, pitchRate = 0, yawRate = 0;      // deg/s

// what the app wants
float cmdRoll = 0, cmdPitch = 0, cmdYaw = 0;         // deg, deg, deg/s
float cmdThrust = 0;                                 // 0 to 1
bool unlocked = false;                               // i need a zero thrust packet first
unsigned long lastPacketMs = 0;

// battery
float vbatMv = 4000;
bool lowBat = false;
unsigned long lowBatSince = 0;

// motor test over serial
int testMotor = -1;
unsigned long testUntilMs = 0;

unsigned long lastLoopUs = 0;
unsigned long lastSlowMs = 0;

struct PID {
  float kp, ki, kd;
  float integ, lastMeas, dFilt;
};

PID pidRoll  = {RATE_KP, RATE_KI, RATE_KD, 0, 0, 0};
PID pidPitch = {RATE_KP, RATE_KI, RATE_KD, 0, 0, 0};
PID pidYaw   = {YAW_KP,  YAW_KI,  0.0f,    0, 0, 0};

// pid
float pidRun(PID &p, float want, float meas, float dt)
{
  float err = want - meas;
  p.integ += err * p.ki * dt;
  p.integ = constrain(p.integ, -I_LIMIT, I_LIMIT);

  // derivative on the measurement 
  float d = -(meas - p.lastMeas) / dt;
  p.lastMeas = meas;
  p.dFilt += 0.2f * (d - p.dFilt);

  return p.kp * err + p.integ + p.kd * p.dFilt;
}

void pidReset(PID &p)
{
  p.integ = 0;
  p.dFilt = 0;
  p.lastMeas = 0;
}

// motors
void setMotor(int i, float duty)
{
  duty = constrain(duty, 0.0f, 1.0f);
  ledcWrite(motorPin[i], (uint32_t)(duty * PWM_MAX));
}

void allMotorsOff()
{
  for (int i = 0; i < 4; i++) setMotor(i, 0);
}

// mpu6050
void writeReg(uint8_t reg, uint8_t val)
{
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}

bool mpuInit()
{
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(REG_WHO_AM_I);
  if (Wire.endTransmission(false) != 0) return false;
  if (Wire.requestFrom((uint16_t)MPU_ADDR, (size_t)1, true) != 1) return false;
  uint8_t who = Wire.read();
  Serial.printf("who am i: 0x%02X\n", who);  

  writeReg(REG_PWR_MGMT_1, 0x01);     // wake up, clock from the gyro pll
  delay(100);
  writeReg(REG_CONFIG, 0x02);         
  writeReg(REG_GYRO_CONFIG, 0x08);    
  writeReg(REG_ACCEL_CONFIG, 0x08);   
  return true;
}

// reads accel (g) and gyro (deg/s) with x forward, y left, z up
bool readIMU(float &ax, float &ay, float &az, float &gx, float &gy, float &gz)
{
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(REG_ACCEL_XOUT_H);
  if (Wire.endTransmission(false) != 0) return false;
  if (Wire.requestFrom((uint16_t)MPU_ADDR, (size_t)14, true) != 14) return false;

  int16_t v[7];
  for (int i = 0; i < 7; i++) {
    uint8_t hi = Wire.read();
    uint8_t lo = Wire.read();
    v[i] = (int16_t)((hi << 8) | lo);
  }
  ax = v[0] / 8192.0f;       
  ay = v[1] / 8192.0f;
  az = v[2] / 8192.0f;
  gx = v[4] / 65.5f;         
  gy = v[5] / 65.5f;
  gz = v[6] / 65.5f;
  return true;
}

void calibrateGyro()
{
  float sx = 0, sy = 0, sz = 0;
  float sax = 0, say = 0, saz = 0;
  int n = 0;
  float ax, ay, az, gx, gy, gz;

  // the drone has to sit still on the table for this
  for (int i = 0; i < 500; i++) {
    if (readIMU(ax, ay, az, gx, gy, gz)) {
      sx += gx; sy += gy; sz += gz;
      sax += ax; say += ay; saz += az;
      n++;
    }
    delay(2);
  }
  if (n == 0) return;

  gxOff = sx / n;
  gyOff = sy / n;
  gzOff = sz / n;

  // i start the angles from the accelerometer
  ax = sax / n; ay = say / n; az = saz / n;
  roll  = atan2f(ay, az) * RAD2DEG;
  pitch = atan2f(-ax, sqrtf(ay * ay + az * az)) * RAD2DEG;
}

// a udp packet is a crtp packet plus 1 checksum byte (the sum of all the bytes before it)
// in the crtp header the port is the top 4 bits and the channel is the lowest 2 bits
// port 3 channel 0 is the commander setpoint: float roll, float pitch, float yaw, uint16 thrust
void readPackets()
{
  int len = udp.parsePacket();
  if (len <= 0) return;

  uint8_t buf[64];
  if (len > (int)sizeof(buf)) {
    udp.read(buf, sizeof(buf));     // too big, i throw it away
    return;
  }
  udp.read(buf, len);

  uint8_t sum = 0;
  for (int i = 0; i < len - 1; i++) sum += buf[i];
  if (sum != buf[len - 1]) return;  // bad checksum, i ignore it

  uint8_t port = buf[0] >> 4;
  uint8_t chan = buf[0] & 0x03;

  if (port == 3 && chan == 0 && len == 16) {
    float r, p, y;
    uint16_t t;
    memcpy(&r, buf + 1, 4);
    memcpy(&p, buf + 5, 4);
    memcpy(&y, buf + 9, 4);
    memcpy(&t, buf + 13, 2);

    lastPacketMs = millis();

    cmdRoll  = constrain(CMD_ROLL_SIGN  * r, -MAX_ANGLE, MAX_ANGLE);
    cmdPitch = constrain(CMD_PITCH_SIGN * p, -MAX_ANGLE, MAX_ANGLE);
    cmdYaw   = CMD_YAW_SIGN * y;
    cmdThrust = t / 65535.0f;

    // same idea as the crazyflie, i need a zero thrust packet first to unlock
    if (t == 0) unlocked = true;
    if (!unlocked) cmdThrust = 0;
  } else {
    // something else from the app (params, log and so on), i just print it so i can see it
    Serial.printf("other packet: port %d chan %d len %d\n", port, chan, len);
  }
}

// i send 1 to 4 over serial to spin that motor for a second and 0 to stop, props off!
void readSerial()
{
  while (Serial.available()) {
    char c = Serial.read();
    if (c >= '1' && c <= '4') {
      testMotor = c - '1';
      testUntilMs = millis() + 1000;
    } else if (c == '0') {
      testMotor = -1;
    }
  }
  if (testMotor >= 0 && millis() > testUntilMs) testMotor = -1;
}

// control
void controlStep(float dt)
{
  // outer loop, the angle error gives the rotation rate i want
  float wantRollRate  = constrain(ANGLE_KP * (cmdRoll  - roll),  -MAX_RATE, MAX_RATE);
  float wantPitchRate = constrain(ANGLE_KP * (cmdPitch - pitch), -MAX_RATE, MAX_RATE);
  float wantYawRate   = constrain(cmdYaw, -MAX_YAW_RATE, MAX_YAW_RATE);

  // inner loop, the rate error gives how much i change the motors
  float rollOut  = pidRun(pidRoll,  wantRollRate,  rollRate,  dt);
  float pitchOut = pidRun(pidPitch, wantPitchRate, pitchRate, dt);
  float yawOut   = pidRun(pidYaw,   wantYawRate,   yawRate,   dt);

  float base = cmdThrust * MAX_THROTTLE;

  for (int i = 0; i < 4; i++) {
    float m = base + ROLL_MIX[i] * rollOut + PITCH_MIX[i] * pitchOut + YAW_MIX[i] * yawOut;
    if (m < IDLE_DUTY) m = IDLE_DUTY;
    setMotor(i, m);
  }
}

// battery and led
void updateBattery()
{
  float mv = analogReadMilliVolts(PIN_BAT) * 2.0f;   // 10k and 10k divider
  if (mv < 2000) return;                             // no battery connected (usb only), i ignore it

  vbatMv += 0.05f * (mv - vbatMv);                   // slow filter because the motors make it sag

  if (vbatMv < LOW_BAT_MV) {
    if (lowBatSince == 0) lowBatSince = millis();
    if (millis() - lowBatSince > 1500) lowBat = true;   // stays on until i reboot
  } else {
    lowBatSince = 0;
  }
}

void updateLed()
{
  unsigned long ms = millis();
  if (lowBat)         digitalWrite(PIN_LED, (ms / 100) % 2);   // fast blink
  else if (unlocked)  digitalWrite(PIN_LED, HIGH);             // solid when unlocked
  else                digitalWrite(PIN_LED, (ms / 500) % 2);   // slow blink
}

// setup
void setup()
{
  Serial.begin(115200);      // for the super mini i set USB CDC On Boot to enabled

  pinMode(PIN_LED, OUTPUT);

  // motors low first, then pwm
  for (int i = 0; i < 4; i++) {
    pinMode(motorPin[i], OUTPUT);
    digitalWrite(motorPin[i], LOW);
  }
  // arduino esp32 core 3.x, on core 2.x i would use ledcSetup and ledcAttachPin instead
  for (int i = 0; i < 4; i++) {
    ledcAttach(motorPin[i], PWM_FREQ, PWM_BITS);
    ledcWrite(motorPin[i], 0);
  }

  analogReadResolution(12);
  analogSetPinAttenuation(PIN_BAT, ADC_11db);

  Wire.begin(PIN_SDA, PIN_SCL);
  Wire.setClock(400000);

  if (!mpuInit()) {
    Serial.println("mpu6050 not found, check the wiring");
    while (true) {                       // fast blink forever
      digitalWrite(PIN_LED, !digitalRead(PIN_LED));
      delay(80);
    }
  }
  Serial.println("calibrating gyro, keep it still");
  calibrateGyro();

  // i set up the wifi access point, the ip and port are what the esp-drone app expects
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(IPAddress(192, 168, 43, 42), IPAddress(192, 168, 43, 42), IPAddress(255, 255, 255, 0));
  WiFi.softAP(AP_SSID, AP_PASS, 1);
  // WiFi.setTxPower(WIFI_POWER_8_5dBm);   // some super minis only connect properly with this
  udp.begin(UDP_PORT);

  Serial.println("ready");
  lastLoopUs = micros();
}

// main loop
void loop()
{
  readPackets();
  readSerial();

  unsigned long now = micros();
  if (now - lastLoopUs < LOOP_US) return;
  float dt = (now - lastLoopUs) * 1e-6f;
  lastLoopUs = now;
  if (dt > 0.02f) dt = 0.02f;

  float ax, ay, az, gx, gy, gz;
  if (!readIMU(ax, ay, az, gx, gy, gz)) {
    allMotorsOff();                      
    return;
  }

  rollRate  = gx - gxOff;
  pitchRate = gy - gyOff;
  yawRate   = gz - gzOff;

  // complementary filter, mostly gyro with a little accelerometer to stop the drift
  float accRoll  = atan2f(ay, az) * RAD2DEG;
  float accPitch = atan2f(-ax, sqrtf(ay * ay + az * az)) * RAD2DEG;
  roll  = 0.98f * (roll  + rollRate  * dt) + 0.02f * accRoll;
  pitch = 0.98f * (pitch + pitchRate * dt) + 0.02f * accPitch;

  // no packets for half a second means lock again and stop
  if (millis() - lastPacketMs > 500) {
    unlocked = false;
    cmdThrust = 0;
    cmdRoll = 0;
    cmdPitch = 0;
    cmdYaw = 0;
  }

  // flipped over or crashed, cut the motors until it gets unlocked again
  if (fabsf(roll) > 75 || fabsf(pitch) > 75) {
    unlocked = false;
    cmdThrust = 0;
  }

  if (testMotor >= 0 && !unlocked) {
    for (int i = 0; i < 4; i++) setMotor(i, i == testMotor ? TEST_DUTY : 0);
  } else if (unlocked && !lowBat && cmdThrust > MIN_THRUST) {
    controlStep(dt);
  } else {
    allMotorsOff();
    pidReset(pidRoll);
    pidReset(pidPitch);
    pidReset(pidYaw);
  }

  // slow stuff every 100ms
  if (millis() - lastSlowMs >= 100) {
    lastSlowMs = millis();
    updateBattery();
    updateLed();
    // Serial.printf("roll %.1f pitch %.1f vbat %.0f thrust %.2f\n", roll, pitch, vbatMv, cmdThrust);
  }
}
