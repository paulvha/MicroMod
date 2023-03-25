/*
  Based on the original Arduino_APA102 library, but made it working
  (as it did not work as it should) extended with new functions and
  variable and tailor made it for the Sparkfun MicroMod Input and Display
  DEV-16985 (https://www.sparkfun.com/products/16985)

  Version 1.0 / march 2023 / Paulvha
  =================================================================
  This file is part of the Arduino_APA102 library.
  Copyright (c) 2020 Arduino SA. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _SFs_APA102_h
#define _SFs_APA102_h

#include "Arduino.h"

// predefined colors
#define color_white  0xFFFFFF
#define color_black  0x000000
#define color_red    0xFF0000
#define color_green  0x00FF00
#define color_blue   0x0000FF
#define color_orange 0xFF4500
#define color_gold   0xFFA500
#define color_violet 0xFF4478
#define color_purple 0x6a0dad

// Hardware information Sparkfun Micromod input / output board
#define MAXLEDS    6
#define totalLEDs  MAXLEDS

#if defined(ARDUINO_TEENSY_MICROMOD)
  #define dataPin    41
  #define clockPin   40
#else
  #define dataPin    G1
  #define clockPin   G0
#endif

struct APA102_Def {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint8_t intensity;      // set the color depth
  uint8_t brightness;
};

class SFs_APA102 {
public:

  // create constructor
  // numLeds : number of leds (MAX MAXLEDS)
  // newDataPin : DATA pin connection
  // newClockPin : clock pin connection
  SFs_APA102(uint8_t numLEDs, uint8_t newDataPin, uint8_t newClockPin);

  // de-constructor
  ~SFs_APA102();

  // intitialize the pins, blackout and clear leds
  void begin(void);

  // de-initilize pins
  void end(void);

  // update all leds with new information
  void show(void);

  // clear all leds and set max brightness and intensity
  void clear(void);

  // set color for all individual led using 24bit color
  void setPixelColor(uint16_t index, uint32_t newColor);

  // set color for individual led
  void setPixelColor(uint16_t index, uint8_t red, uint8_t green, uint8_t blue);

  // set color for group of leds
  void fill(uint32_t newColor , uint16_t startLed , uint16_t count );

  // set brightness for all leds (max 31)
  void setBrightness(uint8_t newBrightness);

  // set brightness for individual led (max 31)
  void setBrightness(uint8_t index, uint8_t newBrightness);

  // set color strength for all leds (Max 100)
  void setIntensity(uint8_t newIntensity);

  // set color strength for individual led (Max 100)
  void setIntensity(uint8_t index, uint8_t newIntensity);

  // create 32bit color from individual colors
  uint32_t Color(uint8_t newRed, uint8_t newGreen, uint8_t newBlue) {
    return (uint32_t)(newRed << 16 | newGreen << 8 | newBlue);
  }

  // how many leds are addressed
  uint16_t getLeds(){
    return _numLeds;
  }

  // update all the data for an individual led
  void writeBuffer(int index, uint8_t dataR, uint8_t dataG, uint8_t dataB, uint8_t dataIntensity, uint8_t dataBrightness);

private:

  void _write8(uint8_t data);
  void _startFrame(void);
  void _endFrame(void);

  uint8_t _numLeds;
  uint8_t _SCK_pin ;
  uint8_t _SDA_pin ;

  APA102_Def  APA[MAXLEDS]; // hold information for each led
};

#endif
