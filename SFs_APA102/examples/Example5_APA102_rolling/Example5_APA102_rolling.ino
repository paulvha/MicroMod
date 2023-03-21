/*
  This sketch will demonstrate different rolling leds options and different
  colors.

  Based on the original Arduino_APA102 library, but made it working
  (as it did not work as it should) extended with new functions and
  variables and tailor made it for the Sparkfun MicroMod Input and Display
  DEV-16985 (https://www.sparkfun.com/products/16985)

  This will demonstrate different rolling leds options

  Version 1.0 / march 2023 / Paulvha

  Distributed as-is; no warranty is given.
  =================================================================
*/

#include "SFs_APA102.h"

// Construct leds-object
SFs_APA102 leds(totalLEDs, dataPin, clockPin);

#define bright 9         // max 31
#define intensity  50    // max 100
#define UpdateSpeed 100  // number of millis
#define LoopCount 10     // how many cycle for each demo

void setup() {

  Serial.begin(115200);
  Serial.println("Example5_rolling leds test");

  //>>>>>>>>> setup APA102 leds <<<<<<<<<<<<<
  leds.begin();

  leds.setBrightness(bright);
  leds.setIntensity(intensity);
  leds.show();
}

void loop() {
  MovingLeds1(color_blue);
  MovingLeds2(color_red);
  MovingLeds3(color_green);
  MovingLeds4(color_orange);
  MovingLeds5(color_purple);
  MovingLeds6(color_white);
  MovingLeds7(color_gold);
  MovingLeds8(color_violet);
  delay(2000);
}

void MovingLeds1(uint32_t col){
  uint8_t i, j;

  // clear leds
  leds.fill(color_black,0,6);
  leds.show();
  for (j=0; j <  LoopCount ;j++)
  {
    for (i=0; i < 6;i++){
      leds.setPixelColor(i, col);
      delay(UpdateSpeed);
      leds.show();
    }

    for (i=0; i < 6;i++){
      leds.setPixelColor(i, color_black);
      delay(UpdateSpeed);
      leds.show();
    }
  }
}

void MovingLeds2(uint32_t col){
  int8_t i, j;

  // clear leds
  leds.fill(color_black,0,6);
  leds.show();
  for (j=0; j <  LoopCount ;j++)
  {
    for (i=0; i < 6;i++){
      leds.setPixelColor(i, col);
      leds.show();
      delay(UpdateSpeed);
    }

    for (i=6; i > -1;i--){
      leds.setPixelColor(i, color_black);
      leds.show();
      delay(UpdateSpeed);
    }
  }
}

void MovingLeds3(uint32_t col){
  int8_t i, j;

  // clear leds
  leds.fill(color_black,0,6);
  leds.show();
  for (j=0; j <  LoopCount ;j++)
  {
    for (i=0; i < 3;i++){
      leds.setPixelColor(i, col);
      leds.setPixelColor(i+3, col);
      leds.show();
      delay(UpdateSpeed);
    }

    for (i=6; i > -1;i--){
      leds.setPixelColor(i, color_black);
      leds.setPixelColor(i+3, color_black);
      leds.show();
      delay(UpdateSpeed);
    }
  }
}

void MovingLeds4(uint32_t col){
  uint8_t i, j;

  // clear leds
  leds.fill(color_black,0,6);
  leds.show();
  for (j=0; j <  LoopCount ;j++)
  {
    for (i=0; i < 5;i++){
      leds.setPixelColor(i, col);
      leds.setPixelColor(i+1, col);
      delay(UpdateSpeed);
      leds.show();
    }

    for (i=0; i < 5;i++){
      leds.setPixelColor(i, color_black);
      leds.setPixelColor(i+1, color_black);
      delay(UpdateSpeed);
      leds.show();
    }
  }
}

void MovingLeds5(uint32_t col){
  int8_t i, j;

  // clear leds
  leds.fill(color_black,0,6);
  leds.show();
  for (j=0; j <  LoopCount ;j++)
  {
    for (i=0; i < 5;i++){
      leds.setPixelColor(i, col);
      leds.setPixelColor(i+1, col);
      leds.show();
      delay(UpdateSpeed);
    }

    for (i=5; i > 0; i--){
      leds.setPixelColor(i, color_black);
      leds.setPixelColor(i-1, color_black);
      leds.show();
      delay(UpdateSpeed);
    }
  }
}


void MovingLeds6(uint32_t col){
  int8_t i, j;

  // clear leds
  leds.fill(color_black,0,6);
  leds.show();
  for (j=0; j <  LoopCount ;j++)
  {
    for (i=0; i < 5;i++){
      leds.setPixelColor(i, col);
      leds.show();
      delay(UpdateSpeed);
      leds.setPixelColor(i+1, col);
      leds.show();
      delay(UpdateSpeed);
    }

    for (i=5; i > 0;i--){
      leds.setPixelColor(i, color_black);
      leds.show();
      delay(UpdateSpeed);
      leds.setPixelColor(i-1, color_black);
      leds.show();
      delay(UpdateSpeed);
    }
  }
}

void MovingLeds7(uint32_t col){
  int8_t i, j;

  // clear leds
  leds.fill(color_black,0,6);
  leds.show();

  for (j=0; j < LoopCount ;j++)
  {
    for (i=0; i < 3;i++){
      leds.setPixelColor(i, col);
      leds.setPixelColor(5-i, col);
      leds.show();
      delay(UpdateSpeed);
    }

    delay(UpdateSpeed);

    for (i=3; i > -1;i--){
      leds.setPixelColor(i, color_black);
      leds.setPixelColor(5-i, color_black);
      leds.show();
      delay(UpdateSpeed);
    }
  }
}


void MovingLeds8(uint32_t col){
  int8_t i, j;

  // clear leds
  leds.fill(color_black,0,6);
  leds.show();

  for (j=0; j < LoopCount ;j++)
  {
    for (i=0; i < 3;i++){
      leds.setPixelColor(i, col);
      leds.setPixelColor(5-i, col);
      leds.show();
      delay(UpdateSpeed);
    }

    delay(UpdateSpeed);

    for (i=0; i < 3;i++){
      leds.setPixelColor(i, color_black);
      leds.setPixelColor(5-i, color_black);
      leds.show();
      delay(UpdateSpeed);
    }
  }
}
