# 3-Inch Quadcopter

The quadcopter is a beginner-friendly drone built with a custom PCB and a custom 3D-printed body. 

The inspiration behind this project was to reignite the interest in drones I had when I was younger, while also serving as a showcase project for my intended majors (mechanical and aeronautical engineering). Beyond this, the project lays the groundwork for a broader vision: building autonomous delivery drones.

## Features
-  Custom PCB featuring an ESP32-C3 development board and a GY-521 IMU (gyroscope/accelerometer).
- Uses high-efficiency MOSFETs to drive the motors.
- Fully controllable via smartphone using the official ESP-Drone application over Wi-Fi.
- Designed with a 2:1 thrust-to-weight ratio to allow for snappy punch-outs.
- Balanced 3D-printed PETG X frame.

## CAD Model
The PCB is secured to the base plate using four M3 screws, with the battery fastened underneath using a Velcro strap. Cutouts on the base plate act as heat sinks for the MOSFETs. My name is embossed on the top plate.

<img src="assets/Drone body pic.png" alt="Model" />

*Designed in Autodesk Fusion 360.*

## PCB
The custom PCB was designed in KiCad. Power traces are wider  to safely handle high current loads. The board dimensions are 70mm × 70mm cuz I avoided surface mount components.

### Schematic
<img src="assets/schema 3.PNG" alt="Schematic" />

### PCB Layout
<img src="assets/drone pic 10.PNG" alt="PCB Layout" />

### 3D Render
<img src="assets/pcb 3d 2.PNG" alt="3D Render" />

## Firmware Overview
The firmware is written in the Arduino IDE. It implements a deterministic PID control loop and Wi-Fi initialization to work with the ESP-Drone mobile app.

## AI Usage
While I tried to keep AI use to a minimum, AI tools were utilized for:
- Researching and finalizing all electronic components.
- Helping with the firmware architecture.

## Experience
Building this project has been an incredible learning experience. I gained knowledge abou power electronics e.g capacitors, MOSFETs, and circuit protection. There were multiple design pivots throughout the development process (documented in `journal.md`). What I originally planned as a straightforward wiring exercise of connecting motors, an ESP32-C3, and a gyro truned into designing a detailed, PCB with over 30 components. Learning how to read manufacturer datasheets and apply them directly to hardware design was one of the most valuable takeaways of the project.

---

## Bill of Materials (BOM)

| Part | Type | Quantity | Unit price (USD) | Line total (USD) | Link | Vendor | Funding | Notes | CART |
| :--- | :--- | :---: | :---: | :---: | :--- | :--- | :--- | :--- | :--- |
| XT30U Male/Female Connectors (5 Pairs) | Part | 1 | $1.12 | $1.12 | https://s.click.aliexpress.com/e/_c3889eTv | Shop1105163493 Store | Grant | | AliExpress |
| 1W Metal Film Resistor 100 Ohm (20pcs pack) | Part | 1 | $1.97 | $1.97 | https://s.click.aliexpress.com/e/_c3E3aj0J | FXI Electronics Co. Ltd. Store | Grant | Lowest quantity sold together | AliExpress |
| 1W Metal Film Resistor 4.7K Ohm (20pcs pack) | Part | 1 | $1.94 | $1.94 | https://s.click.aliexpress.com/e/_c4SeKWnt | FXI Electronics Co. Ltd. Store | Grant | Lowest quantity sold together | AliExpress |
| 1W Metal Film Resistor 1K Ohm (20pcs pack) | Part | 1 | $1.97 | $1.97 | https://s.click.aliexpress.com/e/_c3FJaONV | FXI Electronics Co. Ltd. Store | Grant | Lowest quantity sold together | AliExpress |
| 75mm 2-Blade Propellers 4 Pairs | Part | 1 | $2.40 | $2.40 | https://s.click.aliexpress.com/e/_c3Jr3oTh | RC Club Store | Grant | | AliExpress |
| M3x5mm Nylon Pan Head Screws (50pcs pack) | Part | 1 | $0.93 | $0.93 | https://www.aliexpress.com/item/1005003145381687.html?spm=a2g0o.cart.0.0.e6cc38daaiHVPw&mp=1&pdp_npi=6%40dis%21USD%21USD+0.98%21USD+0.93%21%21USD+0.93%21%21%21%400be71e1917946913243825978e105e%2112000024341162464%21ct%21PK%216665188549%21%211%210%21 | HZYUEGOU SCREWHOME Store | Grant | Lowest quantity sold together | AliExpress |
| 16V 470uF Electrolytic Capacitor (5pcs pack) | Part | 1 | $1.83 | $1.83 | https://s.click.aliexpress.com/e/_c3mb84rh | shenzhenYida Store | Grant | Lowest quantity sold together | AliExpress |
| 16V 47uF Electrolytic Capacitor (5pcs pack) | Part | 1 | $1.69 | $1.69 | https://s.click.aliexpress.com/e/_c3Ba7dwL | shenzhenYida Store | Grant | Lowest quantity sold together | AliExpress |
| ESP32-C3 SuperMini Development Board | Part | 1 | $2.96 | $2.96 | https://www.aliexpress.com/item/1005008815113485.html?spm=a2g0o.cart.0.0.e6cc38daaiHVPw&mp=1&pdp_npi=6%40dis%21USD%21USD+3.96%21USD+2.96%21%21USD+2.96%21%21%21%400be71e1917946913243825978e105e%2112000053564399094%21ct%21PK%216665188549%21%211%210%21 | Shop1104003965 Store | Grant | | AliExpress |
| SS54 SMA SMD Rectifier Diode (50pcs pack) | Part | 1 | $1.91 | $1.91 | https://s.click.aliexpress.com/e/_c3g7i4M7 | Shop1103320155 Store | Grant | Lowest quantity sold together | AliExpress |
| IRF3708 TO-220 N-Channel MOSFET (10pcs pack) | Part | 1 | $3.49 | $3.49 | https://s.click.aliexpress.com/e/_c3twfGQB | Shop1105738136 Store | Grant | Lowest quantity sold together | AliExpress |
| 100nF Monolithic Ceramic Capacitor (100pcs pack) | Part | 1 | $1.78 | $1.78 | https://www.aliexpress.com/item/1135571256.html?spm=a2g0o.cart.0.0.e6cc38daaiHVPw&mp=1&pdp_npi=6%40dis%21USD%21USD+2.03%21USD+1.78%21%21USD+1.78%21%21%21%400be71e1917946913282516061e105e%2165192300108%21ct%21PK%216665188549%21%211%210%21 | Chinese Super Electronic market | Grant | Lowest quantity sold together | AliExpress |
| 5mm Assorted LED Diode Kit (100pcs total) | Part | 1 | $2.40 | $2.40 | https://www.aliexpress.com/item/1005003395932705.html?spm=a2g0o.cart.0.0.e6cc38daaiHVPw&mp=1&pdp_npi=6%40dis%21USD%21USD+2.78%21USD+2.40%21%21USD+2.40%21%21%21%400be71e1917946913282516061e105e%2112000025597631785%21ct%21PK%216665188549%21%211%210%21 | Pingtoudidi Store | Grant | Lowest quantity sold together | AliExpress |
| 5x20mm PCB Fuse Holder + 10A Fuse (10pcs pack) | Part | 1 | $2.15 | $2.15 | https://s.click.aliexpress.com/e/_c3f8mbJ5 | Ali_CanHui Store | Grant | Lowest quantity sold together | AliExpress |
| 1020 Coreless Motor 3.7V 54000RPM (2pcs pack) | Part | 3 | $3.70 | $11.10 | https://s.click.aliexpress.com/e/_c3aOOkLv | Motor Tool Part Store | Grant | Lowest quantity sold together | AliExpress |
| TPS61023 5V 1A Step-Up Boost Module | Part | 1 | $4.25 | $4.25 | https://s.click.aliexpress.com/e/_c3d6elGB | pb Peggybuy Official Store | Grant | | AliExpress |
| GY-521 MPU-6050 3-Axis Gyro/Accelerometer | Part | 1 | $4.85 | $4.85 | https://s.click.aliexpress.com/e/_c4BQ26QB | Shop1105442666 Store | Grant | | AliExpress |
| GNB TERANTY 1S 720mAh 3.8V 80C LiHV Battery | Part | 1 | $5.39 | $5.39 | https://s.click.aliexpress.com/e/_c31hyBf5 | GAONENG GNB BATTERY Store | Grant | | AliExpress |
| Tax | Tax or fee | 1 | $30.00 | $30.00 | | Customs | Grant | Tax | AliExpress |
| PCB | PCB | 1 | $18.00 | $18.00 | | JLCPCB | Grant | 4 for the board + 14 for the shipping (Cheapest possible) | JLCPCB |
| 3D printed body | 3D printed | 1 | $9.00 | $9.00 | | | Grant | The closest printing legion to me is in Singapore which costs a lot in shipping. That's why purchasing locally is cheaper | |
| AliExpress shipping | Shipping | 1 | $4.00 | $4.00 | | AliExpress | Grant | Most of my items are under free shipping but the battery doesn't have it | AliExpress |
| **Total** | | | | **$115.13** | | | | | |

---

## Info
This project was built for **Fabricate Hack Club**. Any feedback would be appreciated!
