/*
  This sketch will demonstrate with loops the impact of increasing
  brightness. It will set all leds the same and individual led.
  *
  Based on the original Arduino_APA102 library, but made it working
  (as it did not work as it should) extended with new functions and
  variables and tailor made it for the Sparkfun MicroMod Input and Display
  DEV-16985 (https://www.sparkfun.com/products/16985)

  Version 1.0 / march 2023 / Paulvha

  Distributed as-is; no warranty is given.
  =================================================================
*/

#include "SFs_APA102.h"

// Construct object
SFs_APA102 leds(totalLEDs, dataPin, clockPin);

//uint32_t color = random(0, 0xFFFFFF);                    // 0x[R][G][B]
uint32_t color = color_blue;

void setup() {
  Serial.begin(115200);
  Serial.println("Example2_APA102_fade");

  leds.begin();

  // initalize all the leds with the same color
  leds.fill(color, 0, totalLEDs);
  leds.show();
}

void loop() {
  int brightness = 0;

  // ALL FADE
  // FADE IN
  for (brightness = 0; brightness < 101; brightness++ ) {
    leds.setBrightness(brightness);
    leds.show();
    delay(100);
  }
  // FADE OUT
  for (brightness = 100; brightness > -1; brightness-- ) {
    leds.setBrightness(brightness);
    leds.show();
    delay(100);
  }

  //Change color
  //color = random(0, 0xFFFFFF);
  color = color_red;
  leds.fill(color, 0, totalLEDs);

  // INDIVIDUAL FADE
  for (int i = 0; i < totalLEDs; i++) {
    // FADE IN
    for (int brightness = 0; brightness < 101; brightness++) {
      leds.setBrightness(i , brightness);
      delay(100);
      leds.show();
    }
  }

  // INDIVIDUAL FADE
  for (int i = 0; i < totalLEDs; i++) {
    // FADE OUT
    for (int brightness = 100; brightness > -1; brightness--) {
      leds.setBrightness(i , brightness);
      delay(100);
      leds.show();
    }
  }
}
