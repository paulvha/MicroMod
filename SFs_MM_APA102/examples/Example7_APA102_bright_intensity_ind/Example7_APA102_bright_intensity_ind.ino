/*
  Tested for the Sparkfun MicroMod Input and Display DEV-16985 (https://www.sparkfun.com/products/16985)

  This sketch can set the intensity and brightness manually and it will
  show this with the different pre-defined color. It will do so for the
  individual leds. Setting all leds is example6.

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

#define DEFAULT_BRIGHT 9          // max 31 
#define DEFAULT_INTENSITY  50     // max 100
#define SwitchTime  3             // switch every x seconds from color

struct LedStatus{
  uint8_t Brightness;             // brightness between 0 -31
  uint8_t Intensity;              // intensity between 0 - 100
};

LedStatus StatusLeds[totalLEDs]; // store the led information

void setup() {

  Serial.begin(115200);
  Serial.println("\nExample7_APA102_bright_intensity individual leds");
  Serial.println("select first the led: 0 - 5");
  Serial.println("B: bright++\tb: bright--");
  Serial.println("I: Intensity++\ti: Intensity--");

  Serial.println("You can combine e.g. 1BBB => LED 1 brightness +3\n4BBiii => Led 4 brightness +2 & intensity -3");
  Serial.println("0BB3ii => led 0 brightness +2 & Led 3 intensity -2");  
  Serial.println("4BB5II0bbbb => led 4 brightness +2 & Led 5 intensity +2 & Led 0 brightness -4");  
  //>>>>>>>>> setup APA102 leds <<<<<<<<<<<<<
  leds.begin();

  for (uint8_t i = 0; i < totalLEDs; i++) {
    StatusLeds[i].Brightness = DEFAULT_BRIGHT;    // default
    StatusLeds[i].Intensity = DEFAULT_INTENSITY;  // default
  }

  // initalize all the leds with the same color
  Setcolor();
}

void loop() {

  static unsigned st = millis();
  int SelectedLed = -1;            // no led selected

  if (Serial.available()){

    while(Serial.available()){

      delay(20);

      char a = Serial.read();

      switch(a) {
        case 'I':
          SetIntensity(SelectedLed,+1);
          break;
        case 'i':
          SetIntensity(SelectedLed,-1);
          break;
        case 'B':
          SetBrightness(SelectedLed,+1);
          break;
        case 'b':
          SetBrightness(SelectedLed,-1);
          break;
        case '0':         // led selection
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
          SelectedLed = a - 0x30;
          break;
      }
    }

    // set update
    for (uint8_t i = 0; i < totalLEDs; i++) {
      leds.setIntensity(i, StatusLeds[i].Intensity);
      leds.setBrightness(i,StatusLeds[i].Brightness);

      Serial.print("led ");
      Serial.print(i);
      Serial.print(" Brightness: ");
      Serial.print(StatusLeds[i].Brightness);
      Serial.print(" Intensity: ");
      Serial.println(StatusLeds[i].Intensity);
    }
    
    leds.show();
  }

  // switch every x seconds
  if (millis() - st > (SwitchTime * 1000)) {
    Setcolor();
    st = millis();
  }
}

/**
 * update intensity for individual led
 */
void SetIntensity(int led, int ch)
{
  if (led > totalLEDs || led < 0) {
    Serial.print("Invalid led ");
    Serial.println(led);
    return;
  }
  
  if (ch < 0){
    if (StatusLeds[led].Intensity > 0) StatusLeds[led].Intensity--;
  }
  else {
    if(StatusLeds[led].Intensity < 100) StatusLeds[led].Intensity++;
  }
}

/**
 * update brightness for individual led
 */
void SetBrightness(int led, int ch)
{
  if (led > totalLEDs || led < 0) {
    Serial.print("Invalid led ");
    Serial.println(led);
    return;
  }
  if (ch < 0) {
    if (StatusLeds[led].Brightness > 0) StatusLeds[led].Brightness--;
  }
  else {
    if (StatusLeds[led].Brightness < 31) StatusLeds[led].Brightness++;
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
