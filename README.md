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
