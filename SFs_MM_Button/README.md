# Micromod Input and display carrier board Buttons & Buzzer

## ===========================================================

## Background
Having a large number of SparkFun products, I bought a [Micromod Input and display carrier board](https://www.sparkfun.com/products/16985). This one looked interesting with a Hyperdisplay, AP102 leds, buzzer and onboard buttons.

<br>To make the point from beginning: It is a disappointment !!

<br>This product is NOT meeting the standard of other SparkFun products I am used to:
1. The software library for the carrier board as VERY VERY limited. Only showing some squares, triangels and circles on the display. Nothing usefull.
2. NOTHING on how to address the APA102 leds. No documentation, No library, No examples.. NOTHING !!
3. NOTHING on how to use or handle the on-board buttons, No library, NO examples .. NOTHING !!
4. The insight in the source code on GitHub about the button handled by the Attiny84 is NOT WORKING as it is supposed to.
5. An I2C-scanner has to be modified to detect correctly devices due to the on-board Attiny84.

## About THIS library
This library handles the on-board buttons and buzzer. For the APA102 leds and Hyperdisplay I have adjusted / created other libraries.

There a number of challenges with the Attiny84 button handling code, with some difficult to overcome. The de-bounce for the buttons in the Attiny84 does not work as it should, Interrupt when button has been pressed & clicked does not work work as it should, change I2C address does not work, storing changes in the Attiny84 EEPROM does not work.. in short this is NOT meeting the SparkFun standards I am used too.

This library is based on the original SparkFun [SparkFun_Qwiic_Button_Arduino_Library](https://github.com/sparkfun/SparkFun_Qwiic_Button_Arduino_Library) but that does not work for this board. I have removed large parts, made a substantional number of modifications and new examples to make it to work.  Many, many changes to overcome the shortcomings that are part of the board and code, BUT you can use them now with working examples. Also different processors had different difficulties with the Attiny84. not only due to the button-code programmed in it, but also as the Attiny84 does a clock-stretching. The library code is able to handle most of it.

<br>Brand new examples have been created:
 * Example1_MM_PrintButtonStatus
 * Example2_MM_button_ChangeI2CAddress
 * Example3_MM_Button_leds
 * Example4_MM_Button_leds_blink
 * Example5_Micromod_Buzzer

<br>All have been tested with the following processors boards:
 * MicroMod ESP32 WRL-16781  https://www.sparkfun.com/products/16781
 * MicroMod Teensy DEV-16402 https://www.sparkfun.com/products/16402
 * MicroMod Artemis DEV-16401 https://www.sparkfun.com/products/16401
 * MicroMod SAMD51 DEV-16791 https://www.sparkfun.com/products/16791
 * MicroMod nRF52840 WRL-16984 https://www.sparkfun.com/products/16984

## Prerequisites

### Leds: https://github.com/paulvha/SF_MM_APA102
 * needed for some examples

## Software installation
 * Obtain the zip and install like any other

## Program usage
 * Defined in top of sketches

## Versioning

### Version 1.0.0 / March 2023
 * Initial release with Example1 to Example5

## Author
 * Paul van Haastrecht (paulvha@hotmail.com)

## License
 * This code is in the public domain.
 * Distributed as-is; no warranty is given.

## Acknowledgments
