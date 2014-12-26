CarBrains
=========

Not an awesome name I know, but the idea came about to hack an RC car I had lying around to get two improvments:

   1. Longer Transmission Range
   2. Proportional Control

Introduction
------------
This information is based around an RC car with an RX-2 chip. These may be a common 16 pin chip for RC cars though I'm not sure how common they are. I discovered this chip with a bit of googling and found the PDF which is in the reference directory. 

If your not sure whether your car has this chip or not, just try connecting pin 13 to 11 with a 330R resisitor and check if the car goes forward. If so check pin 10 for backwards and 7 and 8 for left and right respectively.

If you've got this far then you are in luck. With an Arduino you should be able to at least load the code and control the car. If you have a couple of nRF2401s then you are all set, otherwise you could modify the code to suit your transciever. If you do then please let me know so I can add it to this repo for others.

Release history
---------------

### v1.0.0 - Initial

This is the first release point. Assuming you have the car wired up correctly, you should be able to control the RC car over a 9600 baud serial connection to the Arduino using the following commands:

| Command | Description             |
| ------- | -----------             |
| w       | Move straight ahead     |
| s       | Move straight backwards |
| a       | Turn wheels left        |
| d       | Turn wheels right       |
| q       | Speed in y dir + 20%    |
| z       | Speed in y dir - 20%    |
| f       | Stop car                |

Note that the y direction is forwards and backawards where as the x direction is left and right. Speed forwards and backwards can be controlled whereas left and right are hard settings (may only be true for this car).

All of this code is assuming that you are controlling the car over a serial connection and is probably only useful for testing.

### v 2.0.0 - PS2 Controller added

Added PS2 Controller, this should allow us to control the car from a PS2 controller without a serial connection (though still wired).

Download and install this library:
https://github.com/madsci1016/Arduino-PS2X

I also needed to change the pins used to avoid conflicts with the RF module (to come later)

Wiring to the RC car, note this has been changed since the previous release.

| Arduino | Wire     | RC Pin        |
| ------- | -------- | -------       |
| 9       | Yellow   | Forward  (11) |
| 8       | Orange   | Backward (10) |
| 7       | Green    | Left  (7)     |
| 6       | Blue     | Right (6)     |
| GND     | Black    | GND (2)       |

This is the wiring for the PS2 Controller, the wires should match up on most controllers.

| Arduino | PS2 Wire | PS2 Pin      |
| ------- | -------- | ------------ |
| 5       | Yellow   | Attention    |
| 4       | Orange   | Command      |
| 3       | Blue     | Clock        |
| 2       | Brown    | Data         |
| VCC     | Red      | +5V          |
| GND     | Black    | Ground       |

At this point the Playstation controller will work with the Arrow pad, if you hit a button then that will keep going in the direction you press. Square will stop everything and L1/R1 will decrease and increase speed.

It work better if you hit triangle, this will allow it to use the analog sticks instead. The Left stick will move forwards and backwards. At the moment reverse is only half speed and is not proportional. Forward is proportial however.

The Right stick will control left/right and when centered will center the wheels again. This is not proportial at the moment also.

### Next

Next time I'm planning to add the RF modules.

Info from:
http://www.billporter.info/2010/06/05/playstation-2-controller-arduino-library-v1-0/
