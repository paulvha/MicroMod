/******************************************************************************
  Checks whether the button is pressed, and then prints its status over serial!

  With 'LEFT' and 'RIGHT' button select LED.
  For the selected led
    Button 'UP' and 'DOWN' increase or decrease blink speed for the selected led
    With 'A' next color loop through different colors
    With 'B' increase intensity is steps of 10 to 100 and then start from 10 again
    With 'CENTER' button increase brightness to 31 and the start from 5 again.

  Created a SparkFun Special function to handle the button on the Micromod Input and Display carrier board.
  https://www.sparkfun.com/products/16985

  Version 1.0 / march 2023 / Paulvha

  Print it to the serial monitor at 115200 baud.

  Distributed as-is; no warranty is given.

  The pre-programmed button handling code in the on-board Attiny84 is weak and can fail easily when you push 2 buttons
  at the same time on a critical moment. This is especially tricky with the on-board 5 way switch.

  see topic https://github.com/sparkfun/MicroMod_Input_and_Display_Carrier/issues/1

  The library and sketch code tries to mitigate as much as possible, but not a 100%.  Be gentle on the buttons,
  if you push 2 at the same time the on-board Attiny84 button handler can still get out of shape
  and only returns 0, no detection of any button.

  A power off/on will be needed in that case to reset the Attiny84, as it does not use the reset-button

  This combined with an Attiny84 which does a cycle-stretching makes it a weak part of the board.

  Tested with MM Artemis, Teensy, nRF52840, ESP32, SAMD51

******************************************************************************/
#include "SFs_MM_Button.h"
#include "SFs_APA102.h"

// Constructs
SFs_APA102 leds(totalLEDs, dataPin, clockPin);
MM_QwiicButton button;

// set the correct interrupt pin. Defined differently on nearly any MM board
#ifdef ARDUINO_AM_AP3_SFE_ARTEMIS_MICROMOD            // Artemis V 1.2.x
#define INTPIN  I2CINT
#elif (defined ARDUINO_APOLLO3_SFE_ARTEMIS_MM_PB)     // Artemis V 2.2.x
#define INTPIN  I2C_INT
#elif (defined ARDUINO_TEENSY_MICROMOD)               // Teensy
#define INTPIN  29
#elif (defined ARDUINO_ESP32_MICROMOD)                // ESP32
#define INTPIN  I2C_INT
#elif (defined ARDUINO_SAMD51_MICROMOD)               // SAMD51
#define INTPIN  I2CINT
#elif (defined ARDUINO_ARDUINO_NANO33BLE)             // nRF52840
#define INTPIN  15
#else
#error Unknown interrupt pin location
#endif

// >>>>>>>>>>>>>>>>>>>>>> Button variables <<<<<<<<<<<<<<<<<<<<<<<<<<
// Implement debounce in the sketch instead and reduce the function in the Attiny84 on the board
// as much as possible. See top of sketch

#define NumButton 7                // number of buttons
unsigned long debounce[NumButton]; // holds the last time each button was seen
#define DebounceTime 10            // 10ms

bool HandleButtonInterrupt = false;// set when interrupt is detected
bool InHandleButton = false;       // to prevent double trigger of handling buttons

// >>>>>>>>>>>>>>>>>>>>>> leds variables <<<<<<<<<<<<<<<<<<<<<<<<<<<
#define DEFAULT_BRIGHT 9           // max 31
#define DEFAULT_INTENSITY  50      // max 100

struct LedStatus{
  uint8_t ColorStatus;            // color on led
  uint8_t  BlinkDelay;            // how blink seconds
  unsigned long BlinkStart;       // When last switch happened
  bool    LedIsOn;                // Status of led
  uint8_t Brightness;             // brightness between 0 -31
  uint8_t Intensity;              // intensity between 0 - 100
};

LedStatus StatusLeds[totalLEDs]; // store the led information
uint8_t LedSelected = 0;         // holds selected led

// >>>>>>>>>>>>>>>>>>>>>> program variables <<<<<<<<<<<<<<<<<<<<<<
// set to 0 to get less debug messages
#define SHOWDEBUG 1

void setup() {

  Serial.begin(115200);
#if (defined ARDUINO_TEENSY_MICROMOD)
  delay(500);
#else
  while (!Serial) {delay(10);}
#endif

  Serial.println("Example4 Micromod Button, LEDS and Blink");

  Serial.println("With 'LEFT' and 'RIGHT' buttons select a led.");
  Serial.println("For the selected led:");
  Serial.println("\tButton 'UP' and 'DOWN' increase or decrease blink speed for the selected led.");
  Serial.println("\tWith 'A' loop through different colors.");
  Serial.println("\tWith 'B' increase intensity is steps of 10 to 100 and then start from 10 again.");
  Serial.println("\tWith 'CENTER' button increase brightness to 31 and the start from 5 again.");
  Serial.println("Press <enter> to get overview current setting");
  Wire.begin(); //Join I2C bus

  //>>>>>>>>> setup buttons  <<<<<<<<<<<<<
  // enable interrupt pin
  pinMode(INTPIN,INPUT);
  attachInterrupt(INTPIN, ButtonInt ,FALLING);

  // check if button sensor will acknowledge over I2C
  if (button.begin() == false) {
    Serial.println("Device did not acknowledge! Freezing.");
    while (1);
  }

  // only enable pressed interrupt. Also setting Clicked interrupt will not work.
  // see top of sketch
  button.disableClickedInterrupt();

  // init debounce options
  button.setDebounceTime(0);
  for (uint8_t i = 0; i < NumButton; i++) debounce[i]= 0x0;

  //>>>>>>>>> setup APA102 leds <<<<<<<<<<<<<
  leds.begin();

  for (uint8_t i = 0; i < totalLEDs; i++) {
    StatusLeds[i].ColorStatus =  0;              // will set for black color
    StatusLeds[i].BlinkDelay = 0;                // NO blink
    StatusLeds[i].Brightness = DEFAULT_BRIGHT;    // default
    StatusLeds[i].Intensity = DEFAULT_INTENSITY;  // default
    StatusLeds[i].LedIsOn = true;                // Led is On
  }

  // initialize all the leds with the color
  UpdateLedColor();

  Serialflush();
}

void loop() {
  // check if button was pressed and caused an interrupt, and tell us if it is!
  if (HandleButtonInterrupt) HandleButton();

  // update leds
  HandleBlink();

  // if enter pressed display current setting
  if (Serial.available()) DisplayCurrentSetting();

  delay(100); //Don't hammer too hard on the I2C bus... ok a little :-)
}

/**
 * called when interrupt happens on the pin
 */
void ButtonInt(){
  // only set if not handling already
  if (! InHandleButton) HandleButtonInterrupt = true;
}

/**
 * flush any pending input
 */
void Serialflush()
{
  while(Serial.available()){
    Serial.read();
    delay(100);
  }
}

/**
 *  display current settings
 */
void DisplayCurrentSetting()
{
  Serialflush();

  for (uint8_t i = 0; i < totalLEDs; i++) {
    Serial.print("led: ");
    Serial.print(i);
    Serial.print("\tColor: ");
    Serial.print(StatusLeds[i].ColorStatus);
    Serial.print("  Bright: ");
    Serial.print(StatusLeds[i].Brightness);
    Serial.print("\tIntensity: ");
    Serial.print(StatusLeds[i].Intensity);
    Serial.print("\tBlink delay: ");
    Serial.print(StatusLeds[i].BlinkDelay);
    if (StatusLeds[i].LedIsOn) Serial.print("\tON");
    else Serial.print("\tOFF");

    if (StatusLeds[i].BlinkStart > 0){
      Serial.print ("\tNext change: ");
      Serial.println( StatusLeds[i].BlinkDelay - ((millis() - StatusLeds[i].BlinkStart) /1000));
    }
    else
      Serial.println();
  }
  Serial.print("\nCurrent selected led: ");
  Serial.println(LedSelected);
}

/**
 *  This routine will detect the right button pressed.
 *  It will perform debounce detection as there is a bug in the Attiny84 on the Micromod Input and display
 *  see top of sketch
 *
 *  Next to that it will detect whether the button is still pressed after 500mS. If so it will fire a repeat
 *  detection. You do not have to press the same button multiple times, easier and reducing error
 *
 *  Do not call this routine directly from Button Interrupt handler. Some processor libraries do not work
 *
 *  The interrupt will trigger each time a change happens in the buttons-pressed. So if you first press
 *  CENTER an interrupt happens, when you release it a new interrupt happens. But you also get an interrupt when you
 *  press a next button (say A). To prevent re-trigger while handling 'InHandleButton' is set at start and
 *  released at end
 */
void HandleButton()
{
  InHandleButton = true;
  HandleButtonInterrupt = false;

  MM_statusRegisterBitField statusRegister, previous;
  // Initialize
  statusRegister.byteWrapped = 1;
  previous.byteWrapped = 0;
  unsigned long st_repeat = 0;

  while (statusRegister.byteWrapped > 0) {

    // read current state of button(s) pressed
    statusRegister.byteWrapped = button.getStatusRegister();

    // if button status as last time
    if (statusRegister.byteWrapped == previous.byteWrapped) {

      // This will enable repeat button(s) detection while keep holding down
      if (millis() - st_repeat < 500) continue;
    }
    else {
      previous.byteWrapped = statusRegister.byteWrapped & 0x7f; // remove pressed/released bit
    }

    st_repeat = millis();

    if (statusRegister.byteWrapped == 0x80){
      Serial.print("Last Button released\n");
      break;
    }

    if (statusRegister.BTN_A){
      if (millis() - debounce[0] < DebounceTime) continue;
      debounce[0] = millis();
      Serial.println("Button A pressed");
      SetNextColor(LedSelected);
    }

    if (statusRegister.BTN_B){
      if (millis() - debounce[1] < DebounceTime) continue;
      debounce[1] = millis();
      Serial.println("Button B pressed");
      SetNextIntensity(LedSelected);
    }

    if (statusRegister.BTN_DOWN){
      if (millis() - debounce[2] < DebounceTime) continue;
      debounce[2] = millis();
      Serial.println("Button DOWN pressed");
      SetBlinkRate(LedSelected, -10);
    }

    if (statusRegister.BTN_UP){
      if (millis() - debounce[3] < DebounceTime) continue;
      debounce[3] = millis();
      Serial.println("Button UP pressed");
      SetBlinkRate(LedSelected, 10);
    }

    if (statusRegister.BTN_RIGHT){
      if (millis() - debounce[4] < DebounceTime) continue;
      debounce[4] = millis();
      Serial.println("Button RIGHT pressed");
      SetNextLed(-1);
    }

    if (statusRegister.BTN_LEFT){
      if (millis() - debounce[5] < DebounceTime) continue;
      debounce[5] = millis();
      Serial.println("Button LEFT pressed");
      SetNextLed(1);
    }

    if (statusRegister.BTN_CENTER){
      if (millis() - debounce[6] < DebounceTime) continue;
      debounce[6] = millis();
      SetNextBrightness(LedSelected);
    }

    UpdateLedColor();
  }

  // enable new interrupt to this routine
  InHandleButton = false;
}

/**
 * handle blinking led
 */
void HandleBlink()
{

  // for each led
  for (uint8_t i = 0; i < totalLEDs; i++) {

    // if No blink requested on this led (anymore)
    if (StatusLeds[i].BlinkDelay == 0) {
      StatusLeds[i].BlinkStart = 0;              // reset any pending
      StatusLeds[i].LedIsOn = true;

    }
    else if (StatusLeds[i].BlinkStart > 0) {     // if started

      if (millis() - StatusLeds[i].BlinkStart > (StatusLeds[i].BlinkDelay * 1000)) {

        if (StatusLeds[i].LedIsOn) StatusLeds[i].LedIsOn = false ;  // if ON turn off
        else StatusLeds[i].LedIsOn = true;

        StatusLeds[i].BlinkStart = millis();
#if SHOWDEBUG
        Serial.print("Blink led : ");
        Serial.print(i);
        Serial.print(" to: ");
        Serial.println(StatusLeds[i].LedIsOn);
#endif
      }
    }
    else {                                       // start now
      StatusLeds[i].BlinkStart = millis();
#if SHOWDEBUG
      Serial.print("start blink led: ");
      Serial.println(i);
#endif
    }

    if (StatusLeds[i].LedIsOn)   { // set led on
      leds.setBrightness(i, StatusLeds[i].Brightness);
      leds.setIntensity(i, StatusLeds[i].Intensity);
    }
    else {
      leds.setBrightness(i, 0);
      leds.setIntensity(i, 0);
    }
  } // each led

  leds.show();
}

/**
 * update brightness
 */
void SetNextBrightness(uint8_t led)
{
  StatusLeds[led].Brightness += 5;
  if (StatusLeds[led].Brightness > 31) StatusLeds[led].Brightness = 5;
  leds.setBrightness(led, StatusLeds[led].Brightness);
#if SHOWDEBUG
  Serial.print("Selected Brightness: ");
  Serial.println(StatusLeds[led].Brightness);
#endif
}

/**
 * set blink Rate
 */
void SetBlinkRate(uint8_t led, int8_t v)
{
  StatusLeds[led].BlinkDelay += v;
  if (StatusLeds[led].BlinkDelay > 60) StatusLeds[led].BlinkDelay = 5;
  else if (StatusLeds[led].BlinkDelay < 0) StatusLeds[led].BlinkDelay = 0;
#if SHOWDEBUG
  Serial.print("Selected BlinkDelay: ");
  Serial.println(StatusLeds[led].BlinkDelay);
#endif
}

/**
 * Select next led
 */
void SetNextLed(int s)
{
  if (s < 0) {
    if (LedSelected == 0) LedSelected = 5;
    else LedSelected--;
  }
  else {
    if (LedSelected == 5) LedSelected = 0;
    else LedSelected++;
  }
#if SHOWDEBUG
  Serial.print("Selected LED: ");
  Serial.println(LedSelected);
#endif
}

/**
 * loop through colors for the led
 */
void SetNextColor(uint8_t i)
{
  if (StatusLeds[i].ColorStatus < 9) StatusLeds[i].ColorStatus++;
  else StatusLeds[i].ColorStatus = 1;
}

/**
 * update led Intensity
 */
void SetNextIntensity(uint8_t i)
{
  StatusLeds[i].Intensity += 10;
  if (StatusLeds[i].Intensity > 100) StatusLeds[i].Intensity = 10;
  leds.setIntensity(i, StatusLeds[i].Intensity);
#if SHOWDEBUG
  Serial.print("SetNextIntensity LED: ");
  Serial.println(StatusLeds[i].Intensity);
#endif
}

/*
 * Display different colors for each leds
 * The colors are predefined in SFs_APA102.h
 */
void UpdateLedColor(){
  uint32_t col;

  for (uint8_t i = 0; i < totalLEDs; i++) {

    switch(StatusLeds[i].ColorStatus) {
      case 1:
        col = color_white;
        break;
      case 2:
        col = color_red;
        break;
      case 3:
        col = color_green;
        break;
      case 4:
        col = color_blue;
        break;
      case 5:
        col = color_orange;
        break;
      case 6:
        col = color_gold;
        break;
      case 7:
        col = color_purple;
        break;
      case 8:
        col = color_violet;
        break;
      default:
        col = color_black;
        break;
    }

    // fill the color for led
    leds.setPixelColor(i,col);
  }

  // show
  leds.show();
}
