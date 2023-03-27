# Micromod Input and display carrier board

## ===========================================================

## Background
Having a large number of SparkFun products, I bought a [Micromod Input and display carrier board](https://www.sparkfun.com/products/16985). This one looked interesting with a Hyperdisplay, AP102 leds, buzzer and onboard buttons.

<br>To make the point from beginning: It is a disappointment !!

<br>This product is NOT meeting the standard of other SparkFun products I am used to:
1. The software HyperDisplay libraries are VERY VERY limited. Only showing some squares, triangels and circles on the display. Nothing usefull.
2. NOTHING on how to use the APA102 leds. No documentation, No library, No examples.. NOTHING !!
3. NOTHING on how to use or handle the on-board buttons, No library, NO examples .. NOTHING !!
4. NOTHING about the on-board buzzer
5. The insight in the source code on GitHub about the button handled by the Attiny84 is NOT WORKING as it is supposed to.
6. An I2C-scanner has to be modified to detect correctly devices due to the on-board Attiny84.

## NOW what for the Hyperdisplay? -> SFs_MM_InputDisp !
Instead of the SparkFun Hyperdisplay libraries, Adafruit had already created fantastic libraries for hyperdisplay. They also have extensive information on internet and many additional extensions. A number of examples can be used "out of box" by just selecting the right connection points. So have adjusted and included some, but also added my own in combination with APA102 leds, BLE and BME280

## NOW what for the leds?  -> SFs_MM_APA102 !
For the AP102 I have taking an Arduino AP102 library from Github, which turned out already NOT working due to incorrect coding. I have rewritten / adjusted that to make it to work on this carrier board with working examples.

## NOW what for the buttons and Buzzer? -> SFs_MM_Button!
For the buttons I have taken the original "SparkFun Qwiic Button Arduino Library". It does NOT work for this board.  I have removed large parts, made a substantional number of modifications and new examples to make it to work.  Many, many changes to overcome the shortcomings that are part of the board and code, BUT you can use them now with working examples.

For the Buzzer have taken a starting point from Arduino and adjusted for MicroMod Input and Display. It can play 11 different melodies, but it can be extended easily with other melodies of which there many on the Internet.

## For the on-board MicroSD?
The on-board MicroSD can be used as well with a standard [MicroSD filemanager](https://github.com/paulvha/apollo3/tree/master/MicroSD_filemanager) that I created before and can be downloaded. Just select the board for either the Artemis, ESP32 or nRF52840 processor.

## dependencies
* LEDS : https://github.com/paulvha/MicroMod/SFs_MM_APA102
* Button & buzzer : https://github.com/paulvha/MicroMod/SFs_MM_Button
* MicroSD : https://github.com/paulvha/apollo3/tree/master/MicroSD_filemanager
*  depends on https://github.com/greiman/SdFat.git
* Hyperdisplay: https://github.com/paulvha/MicroMod/SFs_MM_InputDisplay
*  depends on https://github.com/adafruit/Adafruit-GFX-Library
*  depends on https://github.com/adafruit/Adafruit_ILI9341
*  depends on https://github.com/adafruit/Adafruit_BusIO
* BLE : https://github.com/arduino-libraries/ArduinoBLE (for example6)

## Author
 * Paul van Haastrecht (paulvha@hotmail.com)

## Version History

### version 1.0.1 / march 2023 / paulvha
 * added Example8 to HyperDisplay

### version 1.0.0 / march 2023 / Paulvha
 * initial version

### License Information

The Sfs_MM_APA102, SFs_MM_button and SFs_MM_Inputdisplay are _**open source**_!

Please review the LICENSE.md file for license information.

Distributed as-is; no warranty is given.

