/*
 *                                            split_vertical  split_horizontal   SPLIT_BOTH_BELOW      SPLIT_BOTH_ABOVE
 * define 1 screen     define 4 screens       2 or 3 screens  2 or 3 screens      3 screens only        3 screens only
 *                          1  |  2              1 | 2 | 3            1              2  |  3              1
 *     1                    ---|----               |   |           -------           -------           --------
 *                          3  |  4                |   |              2                 1               2 | 3
 *                                                                 -------
 *                                                                    3
 * version 1.0 / March 2023 / paulvha
*/

#include "MM_InputDisplay.h"

// constructor for ILI9341 TFT library
Adafruit_ILI9341 tft = Adafruit_ILI9341((int8_t) TFT_CS,(int8_t) TFT_DC,(int8_t) TFT_RST);

SFs_MM::SFs_MM()
{
  // nothing yet
}

void SFs_MM::begin()
{
  tft.begin(SPI_SPEED);

  // seems to be needed for Sparkfun MM to get real colors
  tft.invertDisplay(true);

  ClrDisplay();
}

void SFs_MM::SetBackgroundColor(uint8_t s, uint16_t color){
  if (s > _screens) return;
  screens[s].B_color = color;
}

void SFs_MM::SetTextColor(uint8_t s, uint16_t color){
  if (s > _screens) return;
  screens[s].T_color = color;
}

/*
 * initialise each split dimensions
 */
bool SFs_MM::InitSplScreen(uint8_t NumScreens, uint8_t SplitScreen, uint8_t Rotation)
{
  int16_t  HOR_DIF, VER_DIF, DISP_WIDTH, DISP_HEIGHT = 0;
  int16_t  WIDTH_1, HEIGHT_1, TOPLEFTX_1, TOPY_1 = 0;
  int16_t  WIDTH_2, HEIGHT_2, TOPLEFTX_2, TOPY_2 = 0;
  int16_t  WIDTH_3, HEIGHT_3, TOPLEFTX_3, TOPY_3 = 0;
  int16_t  WIDTH_4, HEIGHT_4, TOPLEFTX_4, TOPY_4 = 0;

  _screens = NumScreens;

  if (NumScreens == 1) {                 // full screen
    HOR_DIF = 1;
    VER_DIF = 1;
  }
  else if (NumScreens == 4) {          // 4 equal equal screens
    HOR_DIF = 2;
    VER_DIF = 2;
  }
  else if (SplitScreen == SPLIT_HORIZONTAL) {
    if (NumScreens == 3) {            // 3 horizontal under each other
      HOR_DIF = 1;
      VER_DIF = 3;
    }
    else {
      HOR_DIF = 1;                    // 2 horizontal under each other
      VER_DIF = 2;
    }
  }
  else if (SplitScreen == SPLIT_VERTICAL)
    if (NumScreens == 3){             // 3 vertical next to each other
      HOR_DIF = 3;
      VER_DIF = 1;
    }
    else {
      HOR_DIF = 2;                    // 2 vertical next to each other
      VER_DIF = 1;
    }

  else if (SplitScreen == SPLIT_BOTH_BELOW || SplitScreen == SPLIT_BOTH_ABOVE)
    if (NumScreens == 3) {
      HOR_DIF = 2;
      VER_DIF = 2;
    }
    else {
      Serial.println("error : can not handle SPLIT_BOTH_XXXX without 3 screens!!");
      return false;
    }

  else {
    Serial.println("error: can not handle the requested split !!");
    return false;
  }

  if (Rotation == ROTATION_LANDSCAPE || Rotation == ROTATION_REV_LANDSCAPE){
    DISP_WIDTH  = 320;
    DISP_HEIGHT = 240;
  }
  else {
    DISP_WIDTH  = 240;
    DISP_HEIGHT = 320;
  }

  // define for each (split) screen
  if (NumScreens == 4) {
    WIDTH_1 =    DISP_WIDTH/HOR_DIF;
    HEIGHT_1 =   DISP_HEIGHT/VER_DIF;
    TOPLEFTX_1 = 0;
    TOPY_1 =     0;

    WIDTH_2 =    DISP_WIDTH/HOR_DIF;
    HEIGHT_2 =   DISP_HEIGHT/VER_DIF;
    TOPLEFTX_2 = WIDTH_1;
    TOPY_2 =     0;

    WIDTH_3 =    DISP_WIDTH/HOR_DIF;
    HEIGHT_3 =   DISP_HEIGHT/VER_DIF;
    TOPLEFTX_3 = 0;
    TOPY_3 =     HEIGHT_1;

    WIDTH_4 =    DISP_WIDTH/HOR_DIF;
    HEIGHT_4 =   DISP_HEIGHT/VER_DIF;
    TOPLEFTX_4 = WIDTH_3;
    TOPY_4 =     HEIGHT_2;
  }
  else if (NumScreens == 2){

    if (SplitScreen == SPLIT_VERTICAL) {

      WIDTH_1 =    DISP_WIDTH/HOR_DIF;
      HEIGHT_1 =   DISP_HEIGHT/VER_DIF;
      TOPLEFTX_1 = 0;
      TOPY_1 =     0;

      WIDTH_2 =    DISP_WIDTH/HOR_DIF;
      HEIGHT_2 =   DISP_HEIGHT/VER_DIF;
      TOPLEFTX_2 = WIDTH_1;
      TOPY_2 =     0;
    }
    else if (SplitScreen == SPLIT_HORIZONTAL) {

      WIDTH_1 =    DISP_WIDTH/HOR_DIF;
      HEIGHT_1 =   DISP_HEIGHT/VER_DIF;
      TOPLEFTX_1 = 0;
      TOPY_1 =     0;

      WIDTH_2 =    DISP_WIDTH/HOR_DIF;
      HEIGHT_2 =   DISP_HEIGHT/VER_DIF;
      TOPLEFTX_2 = 0;
      TOPY_2 =     HEIGHT_1;
    }
    else {
      Serial.println ("Error: incorrect split for 2 screens");
      return false;
    }
  }
  else if (NumScreens == 3) {

    if (SplitScreen == SPLIT_VERTICAL) {
      WIDTH_1 =    DISP_WIDTH/HOR_DIF;
      HEIGHT_1 =   DISP_HEIGHT/VER_DIF;
      TOPLEFTX_1 = 0;
      TOPY_1 =     0;

      WIDTH_2 =    DISP_WIDTH/HOR_DIF;
      HEIGHT_2 =   DISP_HEIGHT/VER_DIF;
      TOPLEFTX_2 = WIDTH_1;
      TOPY_2 =     0;

      WIDTH_3 =    DISP_WIDTH/HOR_DIF;
      HEIGHT_3 =   DISP_HEIGHT/VER_DIF;
      TOPLEFTX_3 = TOPLEFTX_2 + WIDTH_2;
      TOPY_3 =     0;
    }
    else if (SplitScreen == SPLIT_HORIZONTAL) {

      WIDTH_1 =    DISP_WIDTH;
      HEIGHT_1 =   DISP_HEIGHT/VER_DIF;
      TOPLEFTX_1 = 0;
      TOPY_1 =     0;

      WIDTH_2 =    DISP_WIDTH;
      HEIGHT_2 =   DISP_HEIGHT/VER_DIF;
      TOPLEFTX_2 = 0;
      TOPY_2 =     HEIGHT_1;

      WIDTH_3 =    DISP_WIDTH;
      HEIGHT_3 =   DISP_HEIGHT/VER_DIF;
      TOPLEFTX_3 = 0;
      TOPY_3 =     TOPY_2 + HEIGHT_2;
    }
    else if (SplitScreen == SPLIT_BOTH_ABOVE){
      WIDTH_1 =    DISP_WIDTH;
      HEIGHT_1 =   DISP_HEIGHT/VER_DIF;
      TOPLEFTX_1 = 0;
      TOPY_1 =     0;

      WIDTH_2 =    DISP_WIDTH/HOR_DIF;
      HEIGHT_2 =   DISP_HEIGHT/VER_DIF;
      TOPLEFTX_2 = 0;
      TOPY_2 =     HEIGHT_1;

      WIDTH_3 =    DISP_WIDTH/HOR_DIF;
      HEIGHT_3 =   DISP_HEIGHT/VER_DIF;
      TOPLEFTX_3 = WIDTH_2;
      TOPY_3 =     HEIGHT_1;
    }
    else if (SplitScreen == SPLIT_BOTH_BELOW){

      WIDTH_2 =    DISP_WIDTH/HOR_DIF;
      HEIGHT_2 =   DISP_HEIGHT/VER_DIF;
      TOPLEFTX_2 = 0;
      TOPY_2 =     0;

      WIDTH_3 =    DISP_WIDTH;
      HEIGHT_3 =   DISP_HEIGHT/VER_DIF;
      TOPLEFTX_3 = WIDTH_2;
      TOPY_3 =     0;

      WIDTH_1 =    DISP_WIDTH;
      HEIGHT_1 =   DISP_HEIGHT/VER_DIF;
      TOPLEFTX_1 = 0;
      TOPY_1 =     HEIGHT_2;
    }
  }
  else if (NumScreens == 1){ // full screen
    WIDTH_1 =    DISP_WIDTH;
    HEIGHT_1 =   DISP_HEIGHT;
    TOPLEFTX_1 = 0;
    TOPY_1 =     0;
  }

  // initalize screens array
  screens[0].width = WIDTH_1;
  screens[0].height = HEIGHT_1;
  screens[0].topx = TOPLEFTX_1;
  screens[0].topy = TOPY_1;
  screens[0].B_color = BCOLOR_1;
  screens[0].T_color = TCOLOR_1;

  screens[1].width = WIDTH_2;
  screens[1].height = HEIGHT_2;
  screens[1].topx = TOPLEFTX_2;
  screens[1].topy = TOPY_2;
  screens[1].B_color = BCOLOR_2;
  screens[1].T_color = TCOLOR_2;

  screens[2].width = WIDTH_3;
  screens[2].height = HEIGHT_3;
  screens[2].topx = TOPLEFTX_3;
  screens[2].topy = TOPY_3;
  screens[2].B_color = BCOLOR_3;
  screens[2].T_color = TCOLOR_3;

  screens[3].width = WIDTH_4;
  screens[3].height = HEIGHT_4;
  screens[3].topx = TOPLEFTX_4;
  screens[3].topy = TOPY_4;
  screens[3].B_color = BCOLOR_4;
  screens[3].T_color = TCOLOR_4;

  tft.setRotation(Rotation);
  return true;
}

/*
 * set and store current cursor location in a screen
 */
void SFs_MM::StoreCursor(uint8_t s, int16_t x, int16_t y)
{
  screens[s].currentx = x;
  screens[s].currenty = y;

  tft.setCursor(x,y);
}

/*
 * dim the display backligth (b = 255 = max light)
 */
void SFs_MM::setBacklight(uint8_t b)
{
  static bool initpin = true;

  if (initpin) {
    pinMode(bl_pin, OUTPUT);
    initpin= false;
  }

  analogWrite(bl_pin, 255-b);
}

/*
 * clear postions from current location
 * s = screens
 * l = number of digits
 * ts = text size ( * 6 x 8 pixels)
 */
void SFs_MM::clrPos (uint8_t s, int16_t l, uint8_t ts)
{
  tft.fillRect(screens[s].currentx, screens[s].currenty, l * 6 * ts, ts*8, screens[s].B_color);
}

/*
 * clear a splitscreen
 */
void SFs_MM::ClsSplitScreen(uint8_t s)
{
  tft.fillRect(screens[s].topx, screens[s].topy, screens[s].width, screens[s].height, screens[s].B_color);
}

/*
 * clear current line in splitscreen
 */
void SFs_MM::ClsSplitScreenLine(uint8_t s, uint8_t ts)
{
  tft.fillRect(screens[s].topx, screens[s].currenty, screens[s].width, ts*8, screens[s].B_color);
}

/*
 * clear complete screen with given color
 */
void SFs_MM::ClrDisplay(uint16_t color)
{
    tft.fillScreen(color);
}

/*
 * clear postions and set new text on that location
 * s = screens
 * ts = text size
 * p = string to print
 * x = x-location
 * y = y-location
 * l = number of digits to clear
 * c = store CURRENT X location (true), begin of screen X-location (false)
 */
void SFs_MM::clrSetPos(uint8_t s, uint8_t ts, String p, int16_t x, int16_t y, int16_t l, bool c)
{
  screens[s].currentx = x;
  screens[s].currenty = y;
  clrPos(s,l,ts);
  screens[s].currentx = x;
  screens[s].currenty = y;
  AddText(s,ts,p,c);
}

/* start new text in a screen
 *  s = screen number
 *  ts = text size
 *  p = string to print
 *  current = store current X or start of screen X
 */
void SFs_MM::SetText(uint8_t s, uint8_t ts, String p, bool current)
{
  // clear screen
  ClsSplitScreen(s);

  // set cursor with offset to see better the first start
  StoreCursor(s, screens[s].topx, screens[s].topy + OFFSET_Y);

  tft.setTextColor(screens[s].T_color);  tft.setTextSize(ts);
  tft.print(p);

  // store and set cursor in current screen
  if(current) StoreCursor(s,tft.getCursorX(),tft.getCursorY());
  else StoreCursor(s,screens[s].topx,tft.getCursorY());
}

/* start new text in middle first line screen
 *  s = screen number
 *  ts = text size
 *  p = string to print
 *  current = store current X or start of screen X
 */
void SFs_MM::SetTextMiddle(uint8_t s, uint8_t ts, String p, bool current)
{
  int16_t off = 0;

  // clear screen
  ClsSplitScreen(s);

  // get length of message in pixel
  int16_t pl = p.length() * ts * 6;

  if (pl < screens[s].width)  off = (screens[s].width - pl) / 2;

  // set cursor with offset to see better the first start
  StoreCursor(s, screens[s].topx + off, screens[s].topy + OFFSET_Y);

  tft.setTextColor(screens[s].T_color);  tft.setTextSize(ts);
  tft.print(p);

  // store and set cursor in current screen
  if(current) StoreCursor(s,tft.getCursorX(),tft.getCursorY());
  else StoreCursor(s,screens[s].topx,tft.getCursorY());
}

/*
 * Add text in a screen
 *  s = screen number
 *  ts = text size
 *  p = string to print
 */
void SFs_MM::AddText(uint8_t s, uint8_t ts, String p, bool current)
{
  // reset to current location in current screen
  StoreCursor(s, screens[s].currentx,screens[s].currenty);

  tft.setTextColor(screens[s].T_color);  tft.setTextSize(ts);
  tft.print(p);

  // store and set cursor in current screen
  if(current) StoreCursor(s,tft.getCursorX(),tft.getCursorY());
  else StoreCursor(s,screens[s].topx,tft.getCursorY());

}

/* start new text in middle current line screen
 *  s = screen number
 *  ts = text size
 *  p = string to print
 *  current = store current X or start of screen X
 */
void SFs_MM::AddTextMiddle(uint8_t s, uint8_t ts, String p, bool current)
{
  int16_t off = 0;

  // get length of message in pixel
  int16_t pl = p.length() * ts * 6;

  // obtain offset
  if (pl < screens[s].width)  off = (screens[s].width - pl) / 2;

  // clear current line
  ClsSplitScreenLine(s, ts);

  // set cursor with offset to see better the first start
  StoreCursor(s, screens[s].topx + off, screens[s].currenty);

  tft.setTextColor(screens[s].T_color);  tft.setTextSize(ts);
  tft.print(p);

  // store and set cursor in current screen
  if(current) StoreCursor(s,tft.getCursorX(),tft.getCursorY());
  else StoreCursor(s,screens[s].topx,tft.getCursorY());
}
