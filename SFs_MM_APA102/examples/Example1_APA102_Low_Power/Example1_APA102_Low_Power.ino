/*
  This sketch will demonstrate the basics on how to instruct the led
  color, brightness and intensity.

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

// to create a random color
//uint32_t color = random(0, 0xFFFFFF);                    // 0x[R][G][B]

void setup() {

  Serial.begin(115200);
  Serial.println("Example1_APA102_Low_Power");

  leds.begin();

  leds.setBrightness(10);
  leds.setPixelColor(0 , color_blue);
  leds.setPixelColor(1 , color_red);
  leds.setPixelColor(2 , color_green);
  leds.setPixelColor(3 , color_orange);
  leds.setPixelColor(4 , color_gold);
  leds.setPixelColor(5 , color_blue);
  leds.show();

  delay(2500);
  // Intensity does not change the Max brightness, you control the color depth  of the APA102
  // With the same Max brightness set in 2 LEDs, you can see different phyisical color depth
  // depending on the use of each or combination of the internal rgb LEDs which each APA102 component has
  // You can see the effect:
  //
  // Brightness can be set from 0-31
  // Intensity from 0% - 100%

  leds.setBrightness(0,5);
  leds.setBrightness(1,31);
  leds.setBrightness(2,0);
  leds.setBrightness(3,1);
  leds.setBrightness(4,20);
  leds.setBrightness(5,1);
  leds.show();

  delay(2500);

  leds.setBrightness(0,0);
  leds.setBrightness(1,0);
  leds.setBrightness(2,0);
  leds.setBrightness(3,0);
  leds.setBrightness(4,0);
  leds.setBrightness(5,0);
  leds.show();

  // show brightness increase on led 0
  for (uint8_t i=0; i < 32 ; i++){
    leds.setBrightness(0,i);
    delay(200);
    leds.show();
  }

  // set led1 to indicate previous action is completed
  leds.setBrightness(1,15);
  leds.show();

  // decrease brightness slowly on led 0
  for (int8_t i=31; i > 0 ; i--){
    leds.setBrightness(0,i);
    delay(200);
    leds.show();
  }

  // set led2 to indicate previous action is completed
  leds.setBrightness(2,5);
  leds.show();

  delay(2500);

  leds.setBrightness(0,10);

  // increase intensity slowly on led 0
  for (uint8_t i=0; i < 100 ; i++){
    leds.setIntensity(0,i);
    delay(200);
    leds.show();
  }

  // set led3 to indicate previous action is completed
  leds.setBrightness(3,5);
  leds.show();
}

void loop() {
  // put your main code here, to run repeatedly:

}
