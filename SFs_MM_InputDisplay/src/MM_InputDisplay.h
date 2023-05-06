#ifndef _MM_InputDisplay_h
#define _MM_InputDisplay_h

#include "SFs_APA102.h"
#include <Adafruit_GFX.h>      // include Adafruit graphics library
#include <Adafruit_ILI9341.h>  // include Adafruit ILI9341 TFT library
#include "Arduino.h"

/*
 * depends on https://github.com/paulvha/SF_APA102
 * depends on https://github.com/adafruit/Adafruit-GFX-Library
 * depends on https://github.com/adafruit/Adafruit_ILI9341
 * depends on https://github.com/adafruit/Adafruit_BusIO
 *
 * The Adafruit_BusIO is not used for Artemis/Apollo3 and could be left out if you comment out in Adafruit_GFX.cpp
 *  //#include <Adafruit_I2CDevice.h> // paulvha
 *  //#include <Adafruit_SPIDevice.h>
 */

#if defined(ARDUINO_TEENSY_MICROMOD)

#undef PWM0
#define PWM0 CORE_PIN0_BIT

#undef D0
#define D0 CORE_PIN2_BIT

#undef D1
#define D1 CORE_PIN3_BIT

#endif //ARDUINO_TEENSY_MICROMOD

////////////////  Display connections ////////////////////////////////
#define bl_pin    PWM0         // background light pin
#define SPI_SPEED 48000000     // Requests host uC to use the fastest possible SPI speed up to 48MHz
#define TFT_CS    D0           // TFT CS  Chipselect
#define TFT_RST   -1           // TFT RST No reset used
#define TFT_DC    D1           // TFT DC  Data command

///////////////  Display outline  ////////////////////////////////////
#define ROTATION_PORTRAIT      0    // w= 240 , H = 320
#define ROTATION_LANDSCAPE     3    // w= 320 , h = 240
#define ROTATION_REV_PORTRAIT  2    // w= 240 , H = 320 upside
#define ROTATION_REV_LANDSCAPE 1    // w= 320 , h = 240 upside

#define SPLIT_HORIZONTAL       4    // in case of 1, 2 or 3 screens
#define SPLIT_VERTICAL         5    // in case of 1, 2 or 3 screens
#define SPLIT_BOTH_BELOW       6    // ONLY in case of 3 screens: Screen 1 at the bottom left to right / screen 2 and 3 are next to each other above
#define SPLIT_BOTH_ABOVE       7    // ONLY in case of 3 screens: Screen 1 at the top  left to right / screen 2 and 3 are next to each other below

// An offset for X and Y to show the text better at the start within the split screen
#define OFFSET_Y 2
#define OFFSET_X 2

struct spl_screen{
  int16_t width;
  int16_t height;
  int16_t topx;       // top left x position
  int16_t topy;       // top row
  int16_t currentx;   // current x position
  int16_t currenty;   // current y position
  uint16_t B_color;   // default background color
  uint16_t T_color;   // default text color
};

// define colors
#define BCOLOR_1   ILI9341_BLUE       // background color screen 1
#define TCOLOR_1   ILI9341_WHITE      // default text color screen 1
#define BCOLOR_2   ILI9341_GREEN
#define TCOLOR_2   ILI9341_BLACK
#define BCOLOR_3   ILI9341_BLACK
#define TCOLOR_3   ILI9341_WHITE
#define BCOLOR_4   ILI9341_RED
#define TCOLOR_4   ILI9341_WHITE

class SFs_MM {
public:

  SFs_MM(void);

  void begin();

  void ClrDisplay(uint16_t color = ILI9341_BLACK);

  // set and store current cursor location in a screen
  // s = screen
  // x = X-ax
  // y = Y-ax
  void StoreCursor(uint8_t s, int16_t x, int16_t y);

  int16_t GetCurrentx(uint8_t s){
    return(screens[s].currentx);
  }

  int16_t GetCurrenty(uint8_t s){
    return(screens[s].currenty);
  }

  // set background color
  // s = screen
  // color = color
  void SetBackgroundColor(uint8_t s, uint16_t color);

  // set Text color
  // s = screen
  // color = color
  void SetTextColor(uint8_t s, uint16_t color);

  // InitSplScreens
  // Numscreens  : how many split screens
  // Splitscreen : how is the split defined ( e.g horizontal/vertical etc)
  // Rotation    : landscape or portait etc.
  bool InitSplScreen(uint8_t NumScreens, uint8_t SplitScreen, uint8_t Rotation);

  //clear a splitscreen
  // s = screen
  void ClsSplitScreen(uint8_t s);

  // clear current line in splitscreen
  // s = screens
  // ts = text size
  void ClsSplitScreenLine(uint8_t s, uint8_t ts);

  //clear postions from current location
  // s = screens
  // l = number of digits
  // ts = text size
  void clrPos (uint8_t s, int16_t l, uint8_t ts);

  // clear positions and set new text on that location
  // s = screens
  // ts = text size
  // p = string to print
  // x = x-location
  // y = y-location
  // l = number of digits to clear
  // c = store CURRENT X location (true), begin of screen X-location (false)
  void clrSetPos(uint8_t s, uint8_t ts, String p, int16_t x, int16_t y, int16_t l, bool c);

  //set the display backlight (b = 255 = max light)
  void setBacklight(uint8_t b);

  //start new text in a screen
  //  s = screen number
  //  ts = text size
  //  p = string to print
  void SetText(uint8_t s, uint8_t ts, String p, bool current);

  /* start new text in middle first line screen
   *  s = screen number
   *  ts = text size
   *  p = string to print
   *  current = store current X or start of screen X
   */
  void SetTextMiddle(uint8_t s, uint8_t ts, String p, bool current);

  //Add text in a screen
  //  s = screen number
  //  ts = text size
  //  p = string to print
  void AddText(uint8_t s, uint8_t ts, String p, bool current);

  /* start new text in middle current line screen
   *  s = screen number
   *  ts = text size
   *  p = string to print
   *  current = store current X or start of screen X
   */
  void AddTextMiddle(uint8_t s, uint8_t ts, String p, bool current);

private:
  // array to store screen info (max 4 screens)
  spl_screen screens[4];

  uint8_t _screens;
};

#endif //_MM_InputDisplay_h

