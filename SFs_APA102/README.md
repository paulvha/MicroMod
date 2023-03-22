# Micromod Input and display carrier board: APA102 leds
## ===========================================================

## Background
Having a large number of SparkFun products, I bought a [Micromod Input and display carrier board](https://www.sparkfun.com/products/16985). This one looked interesting with a Hyperdisplay, AP102 leds, buzzer and onboard buttons.

<br>To make the point at beginning: It is a disappointment !!

<br>This product is NOT meeting the standard of other SparkFun products I am used to:
1. The SparkFun software library for the carrier board as VERY VERY limited. Only showing some squares, triangels and circles on the display. Nothing usefull.
2. NOTHING on how to address the APA102 leds. No documentation, No library, No examples.. NOTHING !!
3. NOTHING on how to use or handle the on-board buttons, No library, NO examples .. NOTHING !!
4. The insight in the source code on GitHub about the button handled by the Attiny84 is NOT WORKING as it is supposed to.
5. An I2C-scanner has to be modified to detect correctly devices due to the on-board Attiny84.

<br> pfff Anyway also some good news.. I have created a number of libraries to make it working (as good as it was possible)

## About THIS library
This library handles the on-board leds. For the Buttons & Buzzer and Hyperdisplay I have adjusted / created other libraries.

It is based on the original [Arduino_APA102](https://github.com/arduino-libraries/Arduino_APA102) but that does not work for this board. I have made a substantional number of modifications and new examples to make it to work.

<br>Brand new examples have been created:
 * Example1_APA102_Low_Power
 * Example2_APA102_fade
 * Example3_APA102_Simple_Color
 * Example4_APA102_led_colors
 * Example5_APA102_rolling
 * Example6_APA102_bright_intensity
 * Example7_APA102_bright_intensity_ind

<br>All have been tested with the following processors boards:
 * MicroMod ESP32 WRL-16781  https://www.sparkfun.com/products/16781
 * MicroMod Teensy DEV-16402 https://www.sparkfun.com/products/16402
 * MicroMod Artemis DEV-16401 https://www.sparkfun.com/products/16401
 * MicroMod SAMD51 DEV-16791 https://www.sparkfun.com/products/16791
 * MicroMod nRF52840 WRL-16984 https://www.sparkfun.com/products/16984

## Intensity and brightness
There are 2 different ways to increase the LED color for each seperate led.

<br>Brightness is like increasing the current/voltage and hence the LED light stronger. The brightness is handled within each led. The value can be set between 0 (min) and 31 (max).

<br>Intensity is like increasing the color depth (e.g light RED to dark RED). This is handled by the library which will provide a different RGB-color. The value can be set between 0 (max) and 100 (max). However if it is set lower than 20 it will either not show or pulse.

On this MicroMod board you quickly get "blinded" if you set the leds on maximum. You can use Example6 to test the brightness and intensity setting for all leds, Example7 for individual leds to compare

## Prerequisites

### None

## Software installation
 * Obtain the zip and install like any other.

## Program usage
 * Defined in top of sketches.

## Versioning

### Version 1.0.0 / March 2023
 * Initial release with Example1 to Example7

## Author
 * Paul van Haastrecht (paulvha@hotmail.com)

## License
 * This code is in the public domain.
 * Distributed as-is; no warranty is given.

## Acknowledgments

The original driver [Arduino_APA102](https://github.com/arduino-libraries/Arduino_APA102).
Just be aware that library does not work as it is (has coding issues) and even then it does not work for the MicroMod Input & Output Carrier board. That said it was a good starting point and saved a lot of time.
