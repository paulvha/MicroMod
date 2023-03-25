/*  
  Tested for the Sparkfun MicroMod Input and Display DEV-16985 (https://www.sparkfun.com/products/16985)

  ## Intensity and brightness
  There are 2 different ways to increase the LED color for each seperate led.

  Brightness is like increasing the current/voltage and hence the LED light stronger. The brightness is handled within each led. The value can be set between 0 (min) and 31 (max).

  Intensity is like increasing the color depth (e.g light RED to dark RED). This is handled by the library which will provide a different RGB-color. The value can be set between 0 (max) and 100 (max). However if it is set lower than 20 it will either not show or pulse.

  This is like Example6 in SFs_1APA02 library to set Intensity and brightness for all leds. In SFs_1APA02 library there is an example7 to set this for each led seperately
  
  You can set the intensity and brightness manually and it will show this with the different pre-defined color.
  With capital B you increase the brightness up to 31.
  With lower b you decrease the brightness down to 0.

  With capital I you increase the intensity up to 100.
  With lower i you decrease the intensitry down to 0.

  You can enter multiple letters. e.g. 
  BBBB <enter> will increase brightness by 4.
  bbIII <enter> will decrease brigtness by 2 and increase intensity by 3
  
  Version 1.0 / march 2023 / Paulvha
  =====================================================================================================================
 *
 *                                            split_vertical  split_horizontal   SPLIT_BOTH_BELOW      SPLIT_BOTH_ABOVE 
 * define 1 screen     define 4 screens       2 or 3 screens  2 or 3 screens      3 screens only        3 screens only    
 *                          1  |  2              1 | 2 | 3            1              2  |  3               1
 *     1                    ---|----               |   |           -------           -------            --------
 *                          3  |  4                |   |              2                 1                2 | 3
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

uint8_t bright = 9;      // max 31 
uint8_t intensity = 50;  // max 100

void setup() {
  
  Serial.begin(115200);
  Serial.println("Example3_MMID APA102 leds test");

  //>>>>>>>>> setup APA102 leds <<<<<<<<<<<<<
  leds.begin();

  //>>>>>>>>> setup screen <<<<<<<<<<<<<<<<<<
  MM.begin();

  // set backlight display
  MM.setBacklight(255);

  // init split screens
  MM.InitSplScreen(NUMSCREENS, SPLITSCREEN, ROTATION);

  //>>>>>>>>> start display <<<<<<<<<<<<<<<<<<
  // screen 0, textsize 2 ( * 6x8) = 12x18), text, store current X-location  (true)
  MM.SetTextMiddle(0,2,"Example3\n",true);
  MM.AddTextMiddle(0,2,"APA102 leds test\n\n",true);
  MM.AddText(0,2,"B: Brightness increase 1\nb: Brightness decrease 1\n",true);
  MM.AddText(0,2,"I: Intensity  increase 1\ni: Intensity  decrease 1\n",true);
  updateDisplay();

  // initalize all the leds with the same color
  Setcolor();
}

void loop() {

  static unsigned st = millis();
  
  if (Serial.available()){

    while(Serial.available()){
      
      delay(100);
      
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
     
    updateDisplay();
  }
  
  // switch every 3 seconds led color
  if (millis() - st > 3000) {
    Setcolor();
    st = millis();
  }
}

/*
 * update display and leds
 */
void updateDisplay()
{
  char b[30];
  
  // clear splitscreen 1
  MM.ClsSplitScreen(1);

  sprintf(b," Brightness %d\n",bright);
  // screen 1, textsize 3 ( * 6x8) = 12x18), text, store current X-location  (true)
  MM.SetText(1,3,b,true);

  sprintf(b," Intensity  %d\n\n",intensity);
  // screen 1, textsize 3 ( * 6x8) = 12x18), text, store current X-location  (true)
  MM.AddText(1,3,b,true);

  // set (new) brightness and (new) intensity
  leds.setBrightness(bright);
  leds.setIntensity(intensity);
  
  // display new setting
  leds.show();
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
  
  // fill the color, starting led 0, count 6 leds 
  leds.fill(col,0, 6);

  // show 
  leds.show();
    
  // screen 1, textsize 2 ( * 6x8) = 12x18), text, store current locations  (true)
  MM.AddTextMiddle(1 ,2 , m, true);
}
