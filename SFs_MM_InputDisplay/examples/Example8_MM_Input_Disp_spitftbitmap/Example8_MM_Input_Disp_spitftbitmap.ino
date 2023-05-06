/***************************************************
  This is our Bitmap drawing example for the Adafruit ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  With this sketch you read bmp-files from the MicroSD and display on the HyperDisplay

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution

  ===================================================================

  !!!!!! Make sure to select the right MM processor below. !!!!

  This does not work on a Teensy, but that has it's own spitftbitmap library.

  Adjusted  for MicroMod Input and Display (DEV-16985)
  version 1.0 / March 2023 / paulvha

  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  In the Example8_MM_Input_Disp_sptftbitmap folder there are 4 example
  bmp-files you can use to upload (see step 3 below)

  >>>>>>>>>>>>>>>>>>>>> convert /add a picture <<<<<<<<<<<<<<<<<<<<<

  This is a 4 step procedure I used on Ubuntu.

  >>>> Step 1:
  Open a picture in "mtPaint graphic editor"
  Select "image" => Scale Canvas
  Set size to MAX 240 x 320
  save as XXXX.bmp  (XXXX is the name you choose)

  >>>> Step 2
  Connect the MicroSD with a MicroSD <=> USB converter
  Copy the XXXX.bmp on the MicroSD
  Eject the MicroSD from the filemanager

  >>>> Step 3
  Insert the MicroSD in the MicroMod input and Disp Carrier board

  >>>> Step 4
  Add the filename to display in the loop() below
  Compile and upload
*/

#include <Adafruit_ILI9341.h>  // http://librarymanager/All#Adafruit_ILI9341
                               // http://librarymanager/All#Adafruit_GFX_Library
#include <SPI.h>
#include <SdFat.h> //SdFat v2.2.0 by Bill Greiman: http://librarymanager/All#SdFat_exFAT

////////////////////// Define your processor /////////////////////
#define  MICROMOD_INPUT_DISP_APOLLO3     // MicroMod Input Display carrier board MM Artemis
//#define  MICROMOD_INPUT_DISP_ESP32       // MicroMod Input Display carrier board MM ESP32 (WRL-16781)
//#define  MICROMOD_INPUT_DISP_NRF52840    // MicroMod Input Display carrier board MM nRF52840 (WRL-16984)
//>>>>>>>>>>>>>> Slow with Artemis (SPI overhead) <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//>>>>>>>>>>>>>> Doesn't work for a TEENSY but that has it's own library ! <<<<<<<<<<<<<<<

// TFT display and SD card will share the hardware SPI interface.
// Hardware SPI pins are specific to the Arduino board type and
// cannot be remapped to alternate pins.

#if defined(ARDUINO_TEENSY_MICROMOD)

#undef PWM0
#define PWM0 CORE_PIN0_BIT //3

#undef D0
#define D0 CORE_PIN2_BIT  //4

#undef D1
#define D1 CORE_PIN3_BIT  //5

#define SD_CS     CORE_PIN6_BIT   //10
#endif //ARDUINO_TEENSY_MICROMOD

// MicroMod Input and Display (DEV-16985)
#define TFT_CS    D0           // TFT CS  Chipselect
#define TFT_RST   -1           // TFT RST No reset used
#define TFT_DC    D1           // TFT DC  Data command

#if defined(ARDUINO_ARCH_MBED)
#define SD_CS     SPI_CS      // CS (chip select IOM)
#endif

#if (defined MICROMOD_INPUT_DISP_NRF52840) || defined (MICROMOD_INPUT_DISP_ESP32)
#define  SD_CS    SS          // CS (chip select IOM)
#endif

/////////////////////////  MicroSD card ///////////////////////////////////

// SD_FAT_TYPE = 0 for SdFat/File, 1 for FAT16/FAT32, 2 for exFAT, 3 for FAT16/FAT32 and exFAT.
#define SD_FAT_TYPE 3

#if SD_FAT_TYPE == 1
SdFat32 sd;
File32 sd_root; //current dir
File32 sd_file; //current directory entry
#elif SD_FAT_TYPE == 2
SdExFat sd;
ExFile sd_root; //current dir
ExFile sd_file; //current directory entry
#elif SD_FAT_TYPE == 3
SdFs sd;
FsFile sd_root; //current dir
FsFile sd_file; //current directory entryto
#else // SD_FAT_TYPE == 0
SdFat sd;
File sd_root; //current dir
File sd_file; //current directory entry
#endif  // SD_FAT_TYPE

#define SD_CONFIG SdSpiConfig(SD_CS, SHARED_SPI, SD_SCK_MHZ(24)) // 24MHz

///////////// CONSTRUCTORS //////////////////////////////////

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

void setup(void) {

  Serial.begin(115200);

  while (!Serial) {
    if (millis() > 8000) break;
  }

  // Keep the SD card inactive while working the display.
  pinMode(SD_CS, INPUT_PULLUP);
  delay(200);

  tft.begin();
  tft.invertDisplay(true);  // needed for Micromod

  tft.fillScreen(ILI9341_BLUE);

  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println(F("Welcome on tft Monitor..."));

  Serial.print(F("Initializing SD card..."));
  tft.println(F("Init MicroSD..."));

  // setup MicroSD
  BeginSD();

  Serial.println("OK!");
}

/**
 * init MicroSD card
 */
void BeginSD()
{
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH); //Be sure SD is deselected

  if (sd.begin(SD_CONFIG) == false) // Try to begin the SD card using the correct chip select
  {
    Serial.println(F("SD init failed (first attempt). Trying again...\r\n"));
    tft.println(F("failed 1st attempt..."));
    for (int i = 0; i < 250; i++) //Give SD more time to power up, then try again
    {
      delay(1);
    }

    if (sd.begin(SD_CONFIG) == false) // Try to begin the SD card using the correct chip select
    {
      Serial.println(F("SD init failed (second attempt).\n\tIs card present?\n\tFormatted ?\n\tRight board (both in sketch and IDE) selected during compile ?"));
      Serial.println(F("Please ensure the SD card is formatted correctly using https://www.sdcard.org/downloads/formatter/"));
      digitalWrite(SD_CS, HIGH); //Be sure SD is deselected
      tft.println(F("failed 2st attempt...\nFREEZE"));
      while(1);
    }
  }
}

void loop() {
  tft.fillScreen(ILI9341_GREEN);
  bmpDraw("purple.bmp", 0, 0);
  delay(5000);
  bmpDraw("flowers.bmp", 0, 0);
  delay(5000);
  bmpDraw("cat.bmp", 0, 0);
  delay(5000);
  bmpDraw("MM_Input_Disp.bmp", 0, 0);
  delay(5000);
}

// This function opens a Windows Bitmap (BMP) file and
// displays it at the given coordinates.  It's speed up
// by reading many pixels worth of data at a time
// (rather than pixel by pixel).  Increasing the buffer
// size takes more of the Arduino's precious RAM but
// makes loading a little faster.  20 pixels seems a
// good balance for tiny AVR chips.

// Larger buffers are slightly more efficient, but if
// the buffer is too large, extra data is read unnecessarily.
// For example, if the image is 240 pixels wide, a 100
// pixel buffer will read 3 groups of 100 pixels.  The
// last 60 pixels from the 3rd read may not be used.

#define BUFFPIXEL 80

//===========================================================
// Try Draw using writeRect
void bmpDraw(const char *filename, uint8_t x, uint16_t y) {

  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint16_t buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();
  int16_t  Wi, He;

  uint16_t awColors[320];          // hold colors for one row at a time...

  if((x >= tft.width()) || (y >= tft.height())) return;

  Serial.println();
  Serial.print(F("Loading image '"));
  Serial.print(filename);
  Serial.println('\'');

  if (! sd_file.open(filename, O_READ))
  {
    Serial.print("Error: could not open : ");
    Serial.println(filename);
    return;
  }

  // Parse BMP header
  if(read16() == 0x4D42) { // BMP signature
    Serial.print(F("File size: ")); Serial.println(read32());
    (void)read32(); // Read & ignore creator bytes
    bmpImageoffset = read32(); // Start of image data
    Serial.print(F("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print(F("Header size: ")); Serial.println(read32());
    bmpWidth  = read32();
    bmpHeight = read32();

    if(read16() == 1) { // # planes -- must be '1'
      bmpDepth = read16(); // bits per pixel
      Serial.print(F("Bit Depth: ")); Serial.println(bmpDepth);
      if((bmpDepth == 24) && (read32() == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print(F("Image size: "));
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if((x+w-1) >= tft.width())  w = tft.width()  - x;
        if((y+h-1) >= tft.height()) h = tft.height() - y;

        // try to center picture
        if (tft.width() > w) Wi = (tft.width() - w) / 2;
        else Wi = 0;

        if (tft.height() > h) He = (tft.height() - h) / 2;
        else He = 0;


        for (row=0; row<h; row++) { // For each scanline...

          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if(sd_file.position() != pos) { // Need seek?
            sd_file.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          for (col=0; col<w; col++) { // For each pixel...

            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              sd_file.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }

            // Convert pixel from BMP to TFT format, push to display
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            awColors[col] = tft.color565(r,g,b);
          } // end pixel

          tft.drawRGBBitmap(Wi+0, He+row, awColors , w, 1);
        } // end scanline

        Serial.print(F("Loaded in "));
        Serial.print(millis() - startTime);
        Serial.println(" ms");
      } // end goodBmp
    }
  }

  sd_file.close();
  if(!goodBmp) Serial.println(F("BMP format not recognized."));
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16() {
  uint16_t result;
  if (sd_file.read(&result, 2) == 2) // Read a single character
      return result;
  else
    return 0x0;
}

uint32_t read32() {
  uint32_t result;
  if (sd_file.read(&result, 4) == 4) // Read a single character
      return result;
  else
    return 0x0;
}
