---
title: "Quadcopter"
github: "https://github.com/anaelm1/Basic-Quadcopter"
description: "This is the first project in my drone journey. I am starting off with a basic Quadcopter with a custom PCB, 3D printed body, and a controller. \n\nAfter doing this project, I will start a more advanced FPV drone followed by a delivery drone. The reason for all this is to reignite my interest in drones which I had when I was younger and also use it as a showcase project for my intended majors (mechanical & aeronauticalengineering). "
created_at: "2026-09-22"
total_time: "17h 30m"
---

# October 26, 2026: I researched on the components'  selection

I researched on the required components for the project. After getting a basic idea, I started finalizing each component. There are definitely a lot of boards/chips to choose from so research was needed. 
I ended up finalizing the esp32 and MPU6500 for the flight controller part eventhough I considered a lot of options.

Here is the lapse timelapse: https://lapse.hackclub.com/timelapse/zjKeANbEE17I

![](https://fabricate.hackclub-assets.com/81c362df7fb91187bd572e66e70f86e711788dd5a4d9aca8a3d2dbb341f1be1a/image-png/image.png)

**Total time spent: 30m**

# November 7, 2026: Finalized schematics components 

I have so far read through many component datasheets and researched on the whole schematics. 

The schematics I have finalized is:
- Esp 32 C3 development board with a Large Bulk Capacitor (power rail), and a Ceramic capacitor (near power rails). 
- MPU 6050 with a Large Bulk Capacitor (power rail), a resistor (power rail), and a ceramic capacitor (between power and ground). 
- 4x Coreless motors 8520 with:
    - Logic-level N-MOSFET
    - Resistor (between esp32 and MOSFET)
    - Resistor (between gate and ground)
    - Dioide (across the motor)
    - Ceramic capacitor (across motor terminals)
- Battery connector and 1s lipo battery with Bulk Capacitor (power rail towards motors), P-channel MOSFET (does a diodes job). 
- 3.3V LDO
- Battery monitoring: Voltage divider (two equal resistors) from battery to ADC pin, with a ceremic capacitor at the ADC pin. 
- Status LED with a resistor.

Each things' value is saved separetely. Since, I have finalized this, I am going onto the next step of schematics. 

(didn't really have a picture cuz all of it was research work)

![](https://fabricate.hackclub-assets.com/685744a557759864caa047a78cd7ce8f7681a771110764488f18191d0be6a233/image-png/image.png)

![](https://fabricate.hackclub-assets.com/7ea9cfa997efe4c47fa93441639ba68638da924db791c3a0750930b52b432741/image-png/Capture.PNG)

**Total time spent: 1h 30m**

# November 8, 2026: Did testing in Wokwi

I tested out the main flight controller part in Wokwi. It was kind of fun, and I even spotted a few mistakes in the pin layout. Wokwi doesn't has capcitors and noise check so that might be an issue. I am now learning to read datasheet beacuse Ai is not reliable for this. And I think this would be a great thing to learn. Most of my projects so far have straightforward, but I am hoping to use this project to learn engineering concepts in details like capacitors, MOSFAT, datasheets etc.

![](https://fabricate.hackclub-assets.com/93ee63223621755168519d7cfbc6477f1b1ff0eee3f44a147e7534caaeb451d7/image-png/drone%20pics.PNG)
![](https://fabricate.hackclub-assets.com/11f1bf14997393f9c845745291a765e9b3ff5e8e0a13044e2ee64b2fd75c360e/image-png/drone%20pics%202.PNG)

**Total time spent: 30m**

# November 10, 2026: Worked on Schematics

I started working on the schematics. I read through the datasheet of the esp32 C3 dev board and the gy 521 to finalize their connections. I feel much better finalizing the connections by myself instead of using AI to tell me which pins connects to which. After this connection, I had the issue of power supply where my power supply was 3.7v and both of these use 5v for input. I searched through multiple boost modules trying to find the smallest one for my use, but haven't found one. I will probably continue finding the module and if I can't then I am gonna go with MT3608 as I have used it beforehand. 

Reading datasheet is extremely time consuming, but I will continue doing it since its teaching me a lot of new things about the components I am using.

![](https://fabricate.hackclub-assets.com/7197458c202b5f95c1186675375d05195e84b68a45419993fafa86ecc2e9438c/image-png/drone%20pics%203.PNG)

**Total time spent: 1h 30m**

# November 11, 2026: Built the motor part in schematics

I started the session by finalizing the boost module which I was discussing yesterday. The one I decided was the Adafruit MiniBoost 5v module. After this, the next half hour or so was a wild goose chase of finding the footprint of the module. After giving up the search, I made the symbol and footprint myself which was kinda fun. Then, I focused on the motor logic. I made the schematics of one of the motors and the rest is just copy paste. The circuit uses MOSFET and diodes as I discussed before. The next thing to do is probably adding the capacitors and stuff to deal with noise.

![](https://fabricate.hackclub-assets.com/993d8ac1d4d366d7fe594d58862abc14600358133f5fbb7ea24e36cccecb80d9/image-png/drone%20pic%204.PNG)
![](https://fabricate.hackclub-assets.com/8bee2aee5c13ec94355fb4b50b6c4f9cd027c9ae00591ef641e466c170814bfe/image-png/drone%20pic%205.PNG)
![](https://fabricate.hackclub-assets.com/2178dd9fc8f0570485b9915542973a0d8271fb17fcd92ce4d164e692abc1051b/image-png/drone%20pic%206.PNG)

**Total time spent: 1h 30m**

# November 12, 2026: Completed the schematics 

I started the session by completing the motor routing. After this, I added capacitors to all the places (a lot of them) which needed them, and also made the battery monitor part. Then, I rechecked the board myself and Claude 5 times or more which was really time consuming. So, the schematics is somewhat done. I am still unsure about many things so I am gonna put it on reddit for feedback. 

This project's schematics turned out to be more complex/detailed than I thought and the real challenge is going to be to fitting this into a pcb. Still, it has taught me important concepts which will definitely be helpful in the future. 

Check out the schematics!

![](https://fabricate.hackclub-assets.com/7d0d61fabef1666907be5335c23490db8a4ca87d5b22f5476cb288406176f8d7/image-png/schema%203.PNG)

**Total time spent: 2h**

# November 13, 2026: Assigned footprint

I assigned all the footprint! Honestly, I hate assigning footprints cuz it always takes so long. While choosing them, I also finalized component choices like which model do I wanna use etc. Now that this is done, I am starting work on the pcb itself. I also watched a few videos on how to improve my precison in making pcbs, as I struggled with measurements the last time. 

![](https://fabricate.hackclub-assets.com/9a80f38f4896a6cb7bc41a7cb3c822ab138dec7178e410e6d0e6a6737bed5f09/image-png/drone%20pic%208.PNG)

**Total time spent: 2h**

# November 13, 2026: Placed the components on the pcb

I worked on the PCB design. I started off by adding all the components and then I added the hole. At first I was starting with a 30 mm x 30 mm design, but I soon realized it would be too small. That is why I moved to a 50 mm x 50 mm design, but that too was too small, and by the end I was on a 60 mm x 60 mm board size.

At that point I realized that the MOSFET I had added was the size of the size of the a seed. I would have been cooked if I tried soldering them on with a soldering iron. That's why I switched the MOSFETs to ones which were through hole. After changing the schematics, I realized that there was just not enough space to fit everything inside the 60 mm x 60 mm board, even excluding all the traces and all the rules like keeping the capacitors close.

That is why I researched all the options I had. One of the options was to move the gyroscope on top of the ESP32 to save space, but I could not figure out how that would work and it would be too advanced for me. That is why I am now deciding to switch over to an automatic ESC dev board, and I will use a dev board which controls two motors so I only need two. This will eliminate all the MOSFETs and all the resistors and all the extra stuff for the motors. Even though I wanted to use MOSFETs and advanced components like these, it just would not be possible because if I expand the size anymore, I think it would be too big for the aerodynamics and actually having to fly.

![](https://fabricate.hackclub-assets.com/ba3c437488905f101a336b70a4c3dc2bd0787042e14562224049b5a0c461de47/image-png/drone%20pic%209.PNG)

**Total time spent: 2h**

# November 14, 2026: Completed the electronic part of the PCB

In my last devlog, I talked about how there were too many components for the size of the circuit, and how I thought about switching to a simple flat electronic ESC. However, after researching ESCs, the options I had were very limited because they all had very low current ratings. Their current rating were below 2A, while my motors could easily pull more than 2 amps, so it just was not worth doing.

I thought about it and decided to scrap the 3D body of the drone and make the PCB its own body instead. That is why I switched to a 70 mm x 70 mm board. So, I completed the whole PCB design, including all the wiring and everything else, and also got DRC running with zero errors. I also increased the size of the power lines to 1 mm. 

Now, the only things left for me to do are to clean up the silkscreen layer so that I do not get confused when assembling the board myself, and to add the quadcopter X legs. Once those two things are done, the design will be complete since I am scrapping the 3D model part and focusing on making the PCB its own frame. I also saw this method used in a few videos as well.

![](https://fabricate.hackclub-assets.com/483b6471a6cd9cc798d1c0f40f3636a169b33a6b8420d0031ab9bedc8b44cc5c/image-png/3d.PNG)
![](https://fabricate.hackclub-assets.com/268a6aaf5282a6c455b329f822516ff2133ba4b002e8f32e2c31bf37c88eb177/image-png/drone%20pic%2010.PNG)

**Total time spent: 2h**

# November 14, 2026: Made the 3D model

In my last devlog, I decided that I would not use a 3D body, but rather I would use the PCB as its own body, meaning I would make the arms out of the PCB as well. After researching how durable that would be and what the possibilities were, I realized that it would be too unsafe and there would be a really high risk of the PCB breaking at the X joints.

That is why I have decided to design a really simple 3D body which would host the PCB and extend out to hold the motors, and I built it in Fusion. I also switched up the motors to higher power ones. Now I am using 1020 coreless motors which will generate higher thrust.

I have designed a rough body, but there are a few minor things left to do. The drone is going to be a 3-inch build, which means it will use 3 inch propellers with a 70 mm x 70 mm center to host the PCB, and arms extending about 80 mm out from the center. I have attached the rough design, and after finishing the last few details, it will be fully complete.

![](https://fabricate.hackclub-assets.com/eef60e8de5ad59a709f115b7091d0248522fa879d6c73289407f5b1fc6b8e22e/image-png/image.png)

**Total time spent: 2h**

# November 15, 2026: Completed the 3D model and cleaned up the PCB

In my last devlog, I started designing of a simple 3D body in Fusion to host the PCB and extend out to hold the motors.

I finally completed the 3D body of the drone and verified whether it would actually work aerodynamically, and from what I checked with AI agents like Claude, it should. While working on the body, I added mounting holes, straps, fillets to the sides, and other improvements to refine the design. Even though it is still really simple, I think it is a good starter design, especially since I am very limited due to the weight because my PCB is really big.

Once the 3D body was done, I spent 30 min cleaning up the PCB board and all the text on the silkscreen so I do not run into assembly issues from clutter. After that, I spent around 30 minutes finding parts on AliExpress to cut down costs and finalize the BOM.

At this point, the main tasks left are writing the firmware and doing the basic shipping rules like preparing the file system.

![](https://fabricate.hackclub-assets.com/b63a94f6b1fbb19b84e395d887ed29d8c4d7e7d86703541bfb679359e3337890/image-png/body.PNG)
![](https://fabricate.hackclub-assets.com/d215665cb600c97e3d431008127b9a78c1cc6e30e3786aa825b1ad38b1a0d1ff/image-png/pcb%203d%202.PNG)

**Total time spent: 2h**
