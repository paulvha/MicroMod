# Micromod Input and display carrier board Hyperdisplay

## ===========================================================

## About the SparkFun libraries

You can start with the Sparkfun libraries:
<br>[SparkFun_HyperDisplay](https://github.com/sparkfun/SparkFun_HyperDisplay/archive/master.zip)
<br>[HyperDisplay_ILI9341_ArduinoLibrary](https://github.com/sparkfun/HyperDisplay_ILI9341_ArduinoLibrary/archive/master.zip)
<br>[HyperDisplay_4DLCD-320240_ArduinoLibrary](https://github.com/sparkfun/HyperDisplay_4DLCD-320240_ArduinoLibrary/archive/master.zip)

<br>The SparkFun_HyperDisplay has 3 examples, which do nothing and are actually templates to create your own screens.
<br>The HyperDisplay_ILI9341_ArduinoLibrary has 1 example that does not work (crashes constant).
<br>The HyperDisplay_4DLCD-320240_ArduinoLibrary:
* Example1: Only showing some squares, triangels and circles on the display.
* Example2: Demonstrates how some functions can be used and VERY small text.
* Example3: More functions and the demo for splitscreens
* Example4: Screen in screen demo

Given that only Hyperdisplay documentation examples that are provided, I expect it was somehow the core reason for this carrier board. Disappointed with the libraries. All this means you have to do a lot of work yourself to make something usefull. If you want to use different fonts.. find it out yourself !

## About THIS library
This library handles the Hyperdisplay. For the APA102 leds and button & buzzer I have adjusted / created other libraries.

<br> Instead of the SparkFun Hyperdisplay libraries, Adafruit had already created fantastic libraries for hyperdisplay. They also have extensive information on internet e.g. http://adafruit.github.io/Adafruit-GFX-Library/html/class_adafruit___g_f_x.html. Many examples can be used "out of box" by just selecting the right connection points. So have adjusted and included some, but created some of my own.

## Getting Started

<br>Example1: From Adafruit, which is doing same as Example1 from SparkFun HyperDisplay_4DLCD. Took 5 minutes...
<br>Example2: is also from Adafruit. You can take an existing picture and show that on the HyperDisplay. Instruction on how to do that are in the sketch.
<br>Example3: includes working with the APA102 leds.
<br>Example4: Shows fading of the APA102 LEDS
<br>Example5: If you connect a BME280 it will show on splitscreens(*1) and the leds the status of pressure, temperature, humdity
<br>Example6: same as Example5, but now also show on Splitscreens(*1) the status of BLE (*2) and BME280 as well on the leds.
<br>Example7: show the usage of different standard fonts from Adafruit.
<br>Example8: From Adafruit, read bmp-files from MicroSD and displays on HyperDisplay. Instruction on how to do that are in the sketch.

<br>--1) I have created an overlay library layer on-top-off the Adafruit libraries to keep it simple for the user. You now have splitscreens to show real information from a sketch or sensor with real working examples !! How to use them in real working examples is shown in the Example3 - Example7.
<br>--2) Depends on ArduinoBLE support of the processor board.

**Word of warning**
The display with an Artemis processor is very slow as each pixel is addressed with a seperate SPI call (either SparkFun or AdaFruit) and thus the overhead is high. I would only advice this carrier board with another processor like and ESP32, Teensy or nRF52480 for the display.

## Repository Contents

* **/examples** - Example sketches for the library (.ino). Run these from the Arduino IDE.
* **/src** - Source files for the library (.cpp, .h).
* **keywords.txt** - Keywords from this library that will be highlighted in the Arduino IDE.
* **library.properties** - General library properties for the Arduino package manager.

## Prerequisites
* LEDS : https://github.com/paulvha/MicroMod/SFs_MM_APA102

* Hyperdisplay: https://github.com/paulvha/MicroMod/SFs_MM_InputDisplay
*  depends on https://github.com/adafruit/Adafruit-GFX-Library
*  depends on https://github.com/adafruit/Adafruit_ILI9341
*  depends on https://github.com/adafruit/Adafruit_BusIO

* BLE : https://github.com/arduino-libraries/ArduinoBLE (for example6)

## Software installation
Obtain the zip and install like any other

## Program usage
See top of sketches

## Versioning

### Version 1.0.2 / May 2023
 * added support for Teensy (not fully tested yet)

### Version 1.0.1 / March 2023
 * added Example8

### Version 1.0.0 / March 2023
 * Initial release with Example 1 to 7

## Author
 * Paul van Haastrecht (paulvha@hotmail.com)

## License
See the different dependencies

## Acknowledgments
Adafruit for the great library

