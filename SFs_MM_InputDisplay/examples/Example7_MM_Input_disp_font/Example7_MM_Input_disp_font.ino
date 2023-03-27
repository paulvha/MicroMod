/*
 * MANY, really MANY fonts have been defined to use with the Adafruit GFX library. Just look on GithUb.
 * 
 * This example is just howing a couple of the standard ones and how you can use them when you want
 * 
 * https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts
 * 
 * version 1.0 / March 2023 / paulvha
 * 
 */

#include "Adafruit_ILI9341.h"
#include "Fonts/FreeMonoBold12pt7b.h"   // there are MANY (also on internet)
#include "Fonts/FreeSans12pt7b.h"
#include "Fonts/FreeSerif12pt7b.h"
#include "Fonts/FreeSerifBoldItalic12pt7b.h"
#include "Fonts/FreeSerifItalic12pt7b.h"
#include "Fonts/Org_01.h"
#include "Fonts/Picopixel.h"

// MicroMod Input and Display carrier board
#define bl_pin    PWM0         // background light pin
#define TFT_CS    D0           // TFT CS  Chipselect
#define TFT_RST   -1           // TFT RST No reset used
#define TFT_DC    D1           // TFT DC  Data command

Adafruit_ILI9341 display = Adafruit_ILI9341(TFT_CS, TFT_DC);

void setup() {
  display.begin();
  display.setRotation(3);
  display.invertDisplay(true);    // needed for MicroMod
  
  display.fillScreen(ILI9341_BLACK);
  
  display.setFont(&FreeMonoBold12pt7b);  // choose font
  display.setTextSize(1);    
  display.setTextColor(ILI9341_WHITE);
  display.setTextWrap(false);
  display.setCursor(5,30);   // as per Adafruit convention custom fonts draw up from line so move cursor
  display.println("123456789ABCDEFGHabcdefg");

  display.setFont(&FreeSans12pt7b);  // choose font
  display.setTextSize(1);    
  display.setTextColor(ILI9341_YELLOW);
  display.setTextWrap(false);
  display.setCursor(5,60);   // as per Adafruit convention custom fonts draw up from line so move cursor
  display.println("123456789ABCDEFGHabcdefg");

  display.setFont(&FreeSerif12pt7b);  // choose font
  display.setTextSize(1);    
  display.setTextColor(ILI9341_GREEN);
  display.setTextWrap(false);
  display.setCursor(5,90);   // as per Adafruit convention custom fonts draw up from line so move cursor
  display.println("123456789ABCDEFGHabcdefg");

  display.setFont(&FreeSerifBoldItalic12pt7b);  // choose font
  display.setTextSize(1);    
  display.setTextColor(ILI9341_RED);
  display.setTextWrap(false);
  display.setCursor(5,120);   // as per Adafruit convention custom fonts draw up from line so move cursor
  display.println("123456789ABCDEFGHabcdefg");

  display.setFont(&FreeSerifItalic12pt7b);  // choose font
  display.setTextSize(1);    
  display.setTextColor(ILI9341_BLUE);
  display.setTextWrap(false);
  display.setCursor(5,150);   // as per Adafruit convention custom fonts draw up from line so move cursor
  display.println("123456789ABCDEFGHabcdefg");

  display.setFont(&Org_01);  // choose font
  display.setTextSize(2);    
  display.setTextColor(ILI9341_WHITE);
  display.setTextWrap(false);
  display.setCursor(5,180);   // as per Adafruit convention custom fonts draw up from line so move cursor
  display.println("123456789ABCDEFGHabcdefg");

  display.setFont(&Picopixel);  // choose font
  display.setTextSize(3);    
  display.setTextColor(ILI9341_YELLOW);
  display.setTextWrap(false);
  display.setCursor(5,210);   // as per Adafruit convention custom fonts draw up from line so move cursor
  display.println("123456789ABCDEFGHabcdefg");
  
  display.setFont(); // return to the system font
}

void loop() {  
} 
