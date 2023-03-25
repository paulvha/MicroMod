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

#include "SFs_APA102.h"
#include <Arduino.h>

SFs_APA102::SFs_APA102(uint8_t newNumLeds, uint8_t newDataPin, uint8_t newClockPin)
{
  _numLeds = newNumLeds;
  _SDA_pin = newDataPin;
  _SCK_pin = newClockPin;

  if (_numLeds > MAXLEDS) _numLeds = MAXLEDS;
}

void SFs_APA102::writeBuffer(int index, uint8_t dataR, uint8_t dataG, uint8_t dataB, uint8_t dataIntensity, uint8_t brightness){

  if(index < 0 && index > _numLeds){}
  else{

    APA[index].red   = dataR;
    APA[index].green = dataG;
    APA[index].blue  = dataB;
    APA[index].intensity = dataIntensity;
    APA[index].brightness = brightness;
  }
}

SFs_APA102::~SFs_APA102(){
  end();
}

void SFs_APA102::begin(void){

  pinMode(_SDA_pin, OUTPUT);
  pinMode(_SCK_pin, OUTPUT);

  digitalWrite(_SDA_pin, LOW);
  digitalWrite(_SCK_pin, LOW);

  clear();  // Default data, empty colors, full intensity and Brightness
}

void SFs_APA102::end(void){
  pinMode(_SDA_pin, INPUT);
  pinMode(_SCK_pin, INPUT);
}

// set new pixelcolor for specific led based on 24 bit color
void SFs_APA102::setPixelColor(uint16_t index, uint32_t newColor){
  uint8_t newRed   = uint8_t(newColor >> 16);
  uint8_t newGreen = uint8_t(newColor >> 8);
  uint8_t newBlue  = uint8_t(newColor);

  setPixelColor(index,newRed,newGreen,newBlue);
}

// set new pixelcolor for specific led based on 3 different colors
void SFs_APA102::setPixelColor(uint16_t index, uint8_t red, uint8_t green, uint8_t blue){

  if(index < 0 && index > _numLeds){}else{
    APA[index].red   = red;
    APA[index].green = green;
    APA[index].blue  = blue;
  }
}

// set color to range of leds starting a startled and for 'count' leds
void SFs_APA102::fill(uint32_t newColor, uint16_t startLed, uint16_t count ){
  for (int c = startLed; c < count; c++){
    setPixelColor(c, newColor);
  }
}

// the lower the intensity, the "lighter" the color for all leds
void SFs_APA102::setIntensity(uint8_t newIntensity){  // Set a global Intensity
  for (int i = 0; i< _numLeds;i++){
    setIntensity(i , newIntensity);
  }
}

// the lower the intensity, the "lighter" the color for specific led
void SFs_APA102::setIntensity(uint8_t index, uint8_t newIntensity){  // Set individual LED intensity
  newIntensity &= 0x64; //Max 100(dec)
  APA[index].intensity = newIntensity;
}

// Set brightness for all leds
void SFs_APA102::setBrightness(uint8_t newBrightness){
  for (int i = 0; i< _numLeds;i++){
    setBrightness(i , newBrightness);
  }
}

// Set each LED's brightness
void SFs_APA102::setBrightness(uint8_t index, uint8_t newBrightness){
  newBrightness &= 0x1F; //Max 31(dec)
  APA[index].brightness = newBrightness;
}

// Empty all the LEDs & Default brightness and intensity
void SFs_APA102::clear(){

  for (int c = 0; c < _numLeds; c++){
    APA[c].red   = 0;
    APA[c].green = 0;
    APA[c].blue  = 0;
    APA[c].intensity = 100; // max intensity
    APA[c].brightness = 31; // 0b11111
  }
}

//Protocol blocks

// send the (updated) led information
void SFs_APA102::show(void){

  // 3 blocks of 32 bits:
  // [0x00][0x00][0x00][0x00] - START LED, (startFrame)
  // [LED | Current][B][G][R] - LED data
  //  3bit|5bit      8b 8b 8b
  // [0xFF][0xFF][0xFF][0xFF] - START LED (endFrame) - not all 1s, Adafruit_DotStar found the way to avoid the empty LEDs become white

  _startFrame();

  // Get the data from the buffer
  // Send to the line
  // Each LED has 5 bytes
  for (int c = 0; c < _numLeds; c++) {
    uint8_t Intensity = APA[c].intensity;

    uint16_t newRed =   map(Intensity,0,100,0, APA[c].red);
    uint16_t newGreen = map(Intensity,0,100,0, APA[c].green);
    uint16_t newBlue =  map(Intensity,0,100,0, APA[c].blue);

    uint8_t brightness = APA[c].brightness;

    _write8(uint8_t(0b111 << 5 | brightness));
    _write8(newBlue);
    _write8(newGreen);
    _write8(newRed);
  }

  _endFrame();
}

// Start frame block
void SFs_APA102::_startFrame(){
  for (int i = 0; i<4; i++){
    _write8(0x00);
  }
}

// End frame block
void SFs_APA102::_endFrame(){
  for (int i = 0; i < ((_numLeds + 15) / 16); i++) { //Avoid to set the left LEDs white, taken from Adafruit_DotStar
    _write8(0xFF); //
  }
}

// Send the data to the Line
void SFs_APA102::_write8(uint8_t data){

  // Convert the bit into a state H/L
  for (uint8_t i = 8; i--; data <<= 1) {

    if (data & 0x80) digitalWrite(_SDA_pin, HIGH);
    else digitalWrite(_SDA_pin, LOW);

  // Pulse
	digitalWrite(_SCK_pin, HIGH);
  digitalWrite(_SCK_pin, LOW);
  }
}
