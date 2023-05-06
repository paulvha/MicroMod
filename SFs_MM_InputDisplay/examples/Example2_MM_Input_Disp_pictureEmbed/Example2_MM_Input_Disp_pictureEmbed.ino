/***********************************************************************
  Special port from the original pictureEmbed in the Adafruit ILI9341 library
  for the Sparkfun MicroMod Input and Display (DEV-16985)

  Convert an existing picture by scaling and converting colors to RGB565 code so it
  can be shown on the Hyperdisplay.

  Version 1.0 / March 2023 / paulvha

  >>>>>>>>>>>>>>>>>>>>>>>>>>>> convert a picture <<<<<<<<<<<<<<<<<<<<<<<<<

  This is a 10 step procedure that I am SURE it should be able to be done easier.
  But for now it works and it takes about 10 minutes for a picture.
  Below is what I used on Ubuntu.

  >>>>>>>>>>>>>>>>>>>>>>>>>>> CREATE IMAGE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  >>>>Step 1:
  Open a picture in "mtPaint graphic editor"
  Select "image" => Scale Canvas
  Set size to MAX 240 x 320
  save as XXXX.jpg  (XXXX is the name you choose)
  set JPG Save Quality : 100

  The size of the picture can not be more than 300Kb

  >>>> step 2
  In the webbrowser open : http://www.rinkydinkelectronics.com/t_imageconverter565.php

  NOT THE MONO CONVERTER BUT THE 565 !!!

  >>>> step 3
  click "browse" and look up the local file XXXX.jpg (XXXX is your filename)
  click select
  choose CONVERT TO .c file

  >>>>> step 4
  click "make file"
  If you get an error message it is too large..go back to step 1 and reduce the JPG Save Quality from
  50 to lower number. This is "trial and error" to find the right setting

  >>>>> step 5
  When the conversion has been done click on "Click here to download your file"
  The file is now downloaded on your PC

  >>>>>>>>>>>>>>>>>>>>>>>>>>> IMPORT IMAGE IN SKETCH <<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  >>>>> step 6
  Open the downloaded file. In the top find something like
  //Image Size     : 202x320 pixels

  >>>>> step 7
  Open this sketch in the Arduino IDE.
  Go to tab dragon.h
  in top find 2 lines:
  #define DRAGON_WIDTH  218
  #define DRAGON_HEIGHT 320

  >>>>> step 8
  based on this example change those to :
  #define DRAGON_WIDTH  202
  #define DRAGON_HEIGHT 320

  >>>>> step 9
  Copy content of the array with 'PROGMEM =' in the downloaded file
  TO
  replace the content of dragonBitmap-array in dragon.h

  >>>>> step 10
  Compile and upload


  !!!!!!! The original picture (MMID.JPG and MMID.c) in this example
  is stored in the MMID.zip file with this sketch. !!!!!!!!!!

  Alternative (but I did not use that) https://forum.pjrc.com/threads/35575-Export-for-ILI9341_t3-with-GIMP
*/

#include "SPI.h"
#include <Adafruit_ILI9341.h>
#include "dragon.h"

#if defined(ARDUINO_TEENSY_MICROMOD)

#undef PWM0
#define PWM0 CORE_PIN0_BIT

#undef D0
#define D0 CORE_PIN2_BIT

#undef D1
#define D1 CORE_PIN3_BIT

#endif //ARDUINO_TEENSY_MICROMOD

// MicroMod Input and Display (DEV-16985)
#define TFT_CS    D0           // TFT CS  Chipselect
#define TFT_RST   -1           // TFT RST No reset used
#define TFT_DC    D1           // TFT DC  Data command

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

void setup() {
  tft.begin();
  tft.invertDisplay(true);     // needed for MicroMod
}

void loop(void) {
  int16_t Wi, He;

  for(uint8_t r=0; r<4; r++) {

    tft.setRotation(r);
    tft.fillScreen(ILI9341_BLACK);

    // try to center picture
    if (tft.width() > DRAGON_WIDTH) Wi = (tft.width() - DRAGON_WIDTH) / 2;
    else Wi = 0;

    if (tft.height() > DRAGON_HEIGHT) He = (tft.height() - DRAGON_HEIGHT) / 2;
    else He = 0;

    tft.drawRGBBitmap(Wi, He, (uint16_t *)dragonBitmap, DRAGON_WIDTH, DRAGON_HEIGHT);
    delay(3000);
  }
}
