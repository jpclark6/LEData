# Asynchronous Data Transfer With Arduino Using Light

## Introduction

This was an Arduino project I did as an experiment to see whether I could transfer data from one Arduino to another using light, and if so, how fast could I get it? It uses an LED on one Arduino and a light sensor on a different Arduino. After a lot of experimentation I was able to get it to about 3 kbps data transfer rate of useable data at about 5 kbps gross bit rate. The max working spacing between the sensor and LED is currently about 2 inches.

### See a working video [here.](https://youtu.be/fEUkzCr64Jk)

It uses a protocol I made to sync the devices before every character is sent. The slave device waits for the light to turn on, calculates the clock speed after a few light pulses (can currently take a clock speed between 100 microseconds and about 50 milliseconds), and then synchronizes the signal before every character is read. Once the data is read it displays it on a small screen found here - https://www.adafruit.com/product/1673 .

Data is broken down character by character to binary before being sent with light = 1 and dark = 0. A clock speed is entered in each time before sending data. It currently has a very simple GUI using the Arduino Serial Monitor.

## Setup

Supplies:
1. Arduino Uno
1. Arduino Nano
1. Basic LED (any color)
1. 470 ohm resistor for LED
1. Photo transistor - I used this one https://www.adafruit.com/product/2831 (do not use photo cell)
1. 4 kOhm resistor (for photo transistor)
1. Screen to display received data. I used this one https://www.adafruit.com/product/1673

Load up the receiving_data sketch on the receiving Arduino, and then wire up the screen per the pinout in the sketch, and connect the photo transistor with the readout voltage going to the A0 ADC.

Load up the sending_device sketch on the other Arduino, connect the LED to pin 9 and then to the resistor and ground in serial. Keep this plugged in to the computer and open the serial monitor.

## Images

![](/images/20190111_140950.jpg?raw=true)
![](/images/20190111_140954.jpg?raw=true)

## Why to not use a photo cell

Originally I used a photo cell but started running in to issues after decreasing the clock below 40 ms. After some testing I found that the photo cell takes a long time to dip below my cutoff voltage so I was getting false timing. Here are some readings I took with one Arduino blinking every 250 ms, and the other taking a reading every 1 ms. After upgrading to a photo transistor these problems went away. As far as testing went, the photo transistor had 0 lag time between high and low. Plenty fast for even my fastest clock speeds.

![](/images/photocell.png?raw=true)
![](/images/trans.png?raw=true)
Close ups of the fall time
![](/images/photocell_fall.png?raw=true)
Note even with much higher resolution in time compared to the previous graph there is still no reading that wasn't either high or low despite how it may look.
![](/images/trans_fall.png?raw=true)
