/*  
  Based on the original Arduino_APA102 library, but made it working
  extended and tested for the Sparkfun MicroMod Input and Display
  DEV-16985 (https://www.sparkfun.com/products/16985)

  Will show the fading with brightness for the leds and show the progress on the display
  
  Version 1.0 / march 2023 / Paulvha
  =================================================================
 *
 *                                            split_vertical  split_horizontal   SPLIT_BOTH_BELOW      SPLIT_BOTH_ABOVE 
 * define 1 screen     define 4 screens       2 or 3 screens  2 or 3 screens      3 screens only        3 screens only    
 *                          1  |  2              1 | 2 | 3            1              2  |  3              1
 *     1                    ---|----               |   |           -------           -------           --------
 *                          3  |  4                |   |              2                 1               2 | 3
 *                                                                 -------  
 * valid options for ROTATION                                         3  
 *
  ROTATION_PORTRAIT       // w= 240 , H = 320
  ROTATION_LANDSCAPE      // w= 320 , h = 240
  ROTATION_REV_PORTRAIT   // w= 240 , H = 320 upside
  ROTATION_REV_LANDSCAPE  // w= 320 , h = 240 upside
 *
 * valid options for SplitScreen
 *
  SPLIT_HORIZONTAL       // in case of 1, 2 or 3 screens
  SPLIT_VERTICAL         // in case of 1, 2 or 3 screens
  SPLIT_BOTH_BELOW       // ONLY in case of 3 screens: Screen 1 at the bottom left to right / screen 2 and 3 are next to each other above
  SPLIT_BOTH_ABOVE       // ONLY in case of 3 screens: Screen 1 at the top  left to right / screen 2 and 3 are next to each other below
*/

// DEFINE SPLITSCREEN SETTINGS

#define NUMSCREENS   2
#define SPLITSCREEN  SPLIT_HORIZONTAL
#define ROTATION     ROTATION_LANDSCAPE

////////////////////////////////////////////////////////////////////////////
#include "MM_InputDisplay.h"  // includes SFs_APA102.h for leds
// depends on https://github.com/paulvha/SF_APA102
// depends on https://github.com/adafruit/Adafruit-GFX-Library
// depends on https://github.com/adafruit/Adafruit_ILI9341
// depends on https://github.com/adafruit/Adafruit_BusIO

// Construct leds-object
SFs_APA102 leds(totalLEDs, dataPin, clockPin);

// constructor for MicroMod display
SFs_MM MM;

//uint32_t color = random(0, 0xFFFFFF);                    // 0x[R][G][B]
uint32_t color = color_blue;

#define UpdateSpeed  300      // millis

void setup() {
  Serial.begin(115200);
  Serial.println("Example4_MMID_fade_leds");

  //>>>>>>>>> setup APA102 leds <<<<<<<<<<<<<
  leds.begin();
  
  // initalize all the leds with the same color
  leds.fill(color, 0, totalLEDs);
  
  leds.setBrightness(9);
  
  // update leds
  leds.show();

  //>>>>>>>>> setup screen <<<<<<<<<<<<<<<<<<
  MM.begin();

  // set backlight display
  MM.setBacklight(255);

  // init split screens
  MM.InitSplScreen(NUMSCREENS,SPLITSCREEN,ROTATION);

  // screen 0, textsize 2 ( * 6x8) = 12x18), text, store current X-location  (true)
  MM.SetTextMiddle(0,2,"Example4\n\n",true);
  MM.AddTextMiddle(0,2,"MMID Fade leds\n",true);
}

void loop() {
  char p[10];
  int16_t x, y;
  int brightness;

  // FADE IN
  MM.SetText(1,2,"Fade in:  ", true);
  x = MM.GetCurrentx(1);  // get current x from screen 1
  y = MM.GetCurrenty(1);  // get current Y from screen 1
  
  for (brightness = 0; brightness < 32; brightness += 1 ) {
    sprintf(p,"%d", brightness);
    // screen 1, textsize 2 ( * 6x8) = 12x18), text, saved-X,saved-Y, # digits to clear, store current X-location  (true)
    MM.clrSetPos(1, 2, p, x, y, 3, true);
    
    // set new brightness
    leds.setBrightness(brightness);
    
    // show new setting
    leds.show();
    delay(UpdateSpeed);
  }
  
  // FADE OUT
  MM.AddText(1,2,"\nFade out: ", true);
  x = MM.GetCurrentx(1);  // get current x from screen 1
  y = MM.GetCurrenty(1);  // get current Y from screen 1

  for (brightness = 31; brightness > -1; brightness -= 1 ) {
    sprintf(p,"%d", brightness);
    // Add to screen1, textsize, text, from X-ax, from Y-ax, 3 digits clear, store current X (true)
    MM.clrSetPos(1  ,2 , p, x, y, 3, true);
    leds.setBrightness(brightness);
    leds.show();
    delay(UpdateSpeed);
  }

  // Change color
  //color = random(0, 0xFFFFFF);
  color = color_red;
  leds.fill(color, 0, totalLEDs);

  // INDIVIDUAL LEDS FADE-IN
  MM.AddText(1,2,"\nIndividual fade in:  ", true);
  x = MM.GetCurrentx(1);  // get current x from screen 1
  y = MM.GetCurrenty(1);  // get current Y from screen 1
  
  for (int i = 0; i < totalLEDs; i++) {
    // FADE IN
    for (int brightness = 0; brightness < 32; brightness += 1) {
      sprintf(p,"%d %d", i,brightness);
      MM.clrSetPos(1,2, p, x, y, 5, true);
      
      leds.setBrightness(i , brightness);
      delay(100);
      leds.show();
    }
  }

  // INDIVIDUAL FADE
  MM.AddText(1,2,"\nIndividual fade out: ", true);
  x = MM.GetCurrentx(1);
  y = MM.GetCurrenty(1);
  for (int i = 0; i < totalLEDs; i++) {
    // FADE OUT
    for (int brightness = 31; brightness > -1; brightness -= 1) {
      sprintf(p,"%d %d", i,brightness);
      MM.clrSetPos(1,2, p, x, y, 5, true);
      
      leds.setBrightness(i , brightness);
      delay(100);
      leds.show();
    }
  }

  delay(1000);
}
