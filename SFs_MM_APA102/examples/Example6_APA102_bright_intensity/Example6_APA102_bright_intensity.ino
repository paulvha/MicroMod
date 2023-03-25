/*
  Tested for the Sparkfun MicroMod Input and Display DEV-16985 (https://www.sparkfun.com/products/16985)

  This sketch can set the intensity and brightness manually and it will
  show this with the different pre-defined color. It will do so for all
  leds but this could also be done for each individual in Example7

  Brightness is like increasing the current/voltage and hence the LED light stronger. 
  The brightness is handled within each led. The value can be set between 0 (min) and 31 (max).

  Intensity is like increasing the color depth (e.g light RED to dark RED). 
  This is handled by the library which will provide a different RGB-color. 
  The value can be set between 0 (max) and 100 (max). 
  However if it is set lower than 20 it will either not show or pulse.
  Version 1.0 / march 2023 / Paulvha

  Distributed as-is; no warranty is given.
  =================================================================
*/
#include "SFs_APA102.h"

// Construct leds-object
SFs_APA102 leds(totalLEDs, dataPin, clockPin);

uint8_t bright = 9;      // max 31
uint8_t intensity = 50;  // max 100
#define SwitchTime  3    // switch every x seconds from color

void setup() {

  Serial.begin(115200);
  Serial.println("Example6_APA102_bright_intensity");
  Serial.println("B: bright++\tb: bright--");
  Serial.println("I: Intensity++\ti: Intensity");
  Serial.println("You can combine e.g. BBB => brightness +3 / BBiii => brightness +2 & intensity -3");

  //>>>>>>>>> setup APA102 leds <<<<<<<<<<<<<
  leds.begin();

  // initalize all the leds with the same color
  Setcolor();
}

void loop() {

  static unsigned st = millis();

  if (Serial.available()){

    while(Serial.available()){

      delay(20);

      char a = Serial.read();

      switch(a) {
        case 'I':
          if (intensity < 100) intensity++;
          break;
        case 'i':
          if (intensity > 0) intensity--;
          break;
        case 'B':
          if (bright < 31) bright++;
          break;
        case 'b':
          if (bright > 0) bright--;
          break;
      }
    }

    // set update
    leds.setBrightness(bright);
    leds.setIntensity(intensity);
    leds.show();

    Serial.print("Brightness: ");
    Serial.println(bright);
    Serial.print("Intensity: ");
    Serial.println(intensity);
  }

  // switch every x seconds
  if (millis() - st > (SwitchTime * 1000)) {
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
      sprintf(m," white");
      break;
    case 2:
      col = color_red;
      sprintf(m," red");
      break;
    case 3:
      col = color_green;
      sprintf(m," green");
      break;
    case 4:
      col = color_blue;
      sprintf(m," blue");
      break;
    case 5:
      col = color_orange;
      sprintf(m," orange");
      break;
    case 6:
      col = color_gold;
      sprintf(m," gold");
      break;
    case 7:
      col = color_purple;
      sprintf(m," purple");
      break;
    case 8:
      col = color_violet;
      sprintf(m," violet");
      break;
    default:
      c = 1;
      col = color_black;
      sprintf(m," None");
      break;
  }

  leds.fill(col,0, 6);
  leds.show();
  Serial.println(m);
}
