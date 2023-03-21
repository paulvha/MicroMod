/*
  This sketch is showing that you can set a led color based on pre-defined
  color in the library or create a 24bit color based on individual RGB colors.
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
uint32_t color = color_red;  // Save your color

int r = 0;
int g = 100;
int b = 250;

void setup() {

  Serial.begin(115200);
  Serial.println("Example3_APA102_simple_Color");

  if( !leds.begin()){
    Serial.println("Error during initialize");
    while(1);
  }

  leds.setPixelColor(0 , color);
  leds.setPixelColor(1 , r , g , b);
  leds.show();
}

void loop() {
  // put your main code here, to run repeatedly:

}
