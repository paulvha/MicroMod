/*
  Tested for the Sparkfun MicroMod Input and Display DEV-16985 (https://www.sparkfun.com/products/16985)

  This sketch will demonstrate the different pre-defined colors. You can
  set the brightnes and intensity in the sketch.

  Based on the original Arduino_APA102 library, but made it working
  (as it did not work as it should) extended with new functions and
  variables and tailor made it for the Sparkfun MicroMod Input and Display
  DEV-16985 (https://www.sparkfun.com/products/16985)

  Version 1.0 / march 2023 / Paulvha

  Distributed as-is; no warranty is given.
  =================================================================
*/
#include "SFs_APA102.h"

// Construct leds-object
SFs_APA102 leds(totalLEDs, dataPin, clockPin);

uint8_t bright = 9;      // max 31
uint8_t intensity = 50;  // max 100

void setup() {

  Serial.begin(115200);
  Serial.println("Example4_APA102 led colors");

  //>>>>>>>>> setup APA102 leds <<<<<<<<<<<<<
  leds.begin();

  leds.setBrightness(bright);
  leds.setIntensity(intensity);

  // initalize all the leds with the same color
  Setcolor();
}

void loop() {

  static unsigned st = millis();

  // switch every 3 seconds led color
  if (millis() - st > 3000) {
    Setcolor();
    st = millis();
  }
}

/*
 * rotate different colors for leds
 * The colors are predefined in SFs_APA102.h
 */
void Setcolor(){
  static uint8_t c=1;
  uint32_t col;
  char m[20];

  switch(c++) {
    case 1:
      col = color_white;
      sprintf(m," color_white");
      break;
    case 2:
      col = color_red;
      sprintf(m," color_red");
      break;
    case 3:
      col = color_green;
      sprintf(m," color_green");
      break;
    case 4:
      col = color_blue;
      sprintf(m," color_blue");
      break;
    case 5:
      col = color_orange;
      sprintf(m," color_orange");
      break;
    case 6:
      col = color_gold;
      sprintf(m," color_gold");
      break;
    case 7:
      col = color_purple;
      sprintf(m," color_purple");
      break;
    case 8:
      col = color_violet;
      sprintf(m," color_violet");
      break;
    default:
      c = 1;
      col = color_black;
      sprintf(m," color_black");
      break;
  }

  // fill the colors starting led 0, count 6 leds
  leds.fill(col,0, 6);
  Serial.println(m);

  // show
  leds.show();
}
