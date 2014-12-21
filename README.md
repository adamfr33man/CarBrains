CarBrains
=========

Not an awesome name I know, but the idea came about to hack an RC car I had lying around to get two improvments:

   1. Longer Transmission Range
   2. Proportional Control
    
This information is based around an RC car with an RX-2 chip. These may be a common 16 pin chip for RC cars though I'm not sure how common they are. I discovered this chip with a bit of googling and found the PDF which is in the reference directory. 

If your not sure whether your car has this chip or not, just try connecting pin 13 to 11 with a 330R resisitor and check if the car goes forward. If so check pin 10 for backwards and 7 and 8 for left and right respectively.

If you've got this far then you are in luck. With an Arduino you should be able to at least load the code and control the car. If you have a couple of nRF2401s then you are all set, otherwise you could modify the code to suit your transciever. If you do then please let me know so I can add it to this repo for others.