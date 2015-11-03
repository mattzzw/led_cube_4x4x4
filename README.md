# led_cube_4x4x4

Yet another LED cube. 

I used an Atmel/Arduino Uno with 74hc594n shift registers via hardware SPI. 
Most of the effects are taken from 
http://www.instructables.com/id/Led-Cube-8x8x8/
So credit goes out to Christian Moen and Ståle Kristoffersen for their instructable and their software. I used a lot of the effects code from these guys.

I'm using 3 pins to shift data into 2 daisy chained 8bit shift registers (16 LEDs per plane) and another 4 pins to switch planes via npn transistors.

I used netbeans for coding and building was done with a custom makefile (thanks to Kerry Wong) on a Mac. Hence you will probably have to adjust a couple of paths in the makefile.

Project page is at
http://mattzz.no-ip.org/wiki/Projects/LedCube4x4x4


Have fun,
  /Matthias

