/******************************************************************************
  Checks whether the button is pressed, and then prints its status over serial!

  Also with the A, B, UP, DOWN, LEFT and RIGHT it will loop through different colors on different leds
  The CENTER button will increase brightness to 31 and the start from 5 again.

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

// set the right interrupt pin. Defined differently on nearly any board
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

// Implement debounce in the sketch instead and reduce the function in the Attiny84 on the board
// as much as possible. See top of sketch

#define NumButton 7                // number of buttons
unsigned long debounce[NumButton]; // holds the last time each button was seen
#define DebounceTime 10            // 10ms

bool HandleButtonInterrupt = false;// set when interrupt is detected
bool InHandleButton = false;       // to prevent double trigger of handling buttons

// leds variables
uint8_t bright = 9;      // max 31
uint8_t intensity = 50;  // max 100
uint8_t LedStatus[totalLEDs];

void setup() {

  Serial.begin(115200);
#if (defined ARDUINO_TEENSY_MICROMOD)
  delay(500);
#else
  while (!Serial) {delay(10);}
#endif
  Serial.println("Example3 Micromod Button and LEDS");
  Serial.println("With 'A'      button loop through different colors on led 0");
  Serial.println("With 'B'      button loop through different colors on led 1");
  Serial.println("With 'down'   button loop through different colors on led 2");
  Serial.println("With 'up'     button loop through different colors on led 3");
  Serial.println("With 'right'  button loop through different colors on led 4");
  Serial.println("With 'left'   button loop through different colors on led 5");
  Serial.println("With 'CENTER' will increase brightness to 31 and the start from 5 again");

  Wire.begin(); //Join I2C bus

  //>>>>>>>>> setup button  <<<<<<<<<<<<<
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

  leds.setBrightness(bright);
  leds.setIntensity(intensity);

  for (uint8_t i = 0; i < totalLEDs; i++) LedStatus[i] = 10;    // will set for black.

  // initalize all the leds with the same color
  UpdateLedColor();
}

void loop() {
  // check if button was pressed and caused an interrupt, and tell us if it is!
  if (HandleButtonInterrupt) HandleButton();

  delay(100); //Don't hammer too hard on the I2C bus... ok a little :-)
}

/**
 * called when interrupt happens
 */
void ButtonInt(){
  // only set if not handling already
  if (! InHandleButton) HandleButtonInterrupt = true;
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
      SetNextColor(0);
    }

    if (statusRegister.BTN_B){
      if (millis() - debounce[1] < DebounceTime) continue;
      debounce[1] = millis();
      Serial.println("Button B pressed");
      SetNextColor(1);
    }

    if (statusRegister.BTN_DOWN){
      if (millis() - debounce[2] < DebounceTime) continue;
      debounce[2] = millis();
      Serial.println("Button DOWN pressed");
      SetNextColor(2);
    }

    if (statusRegister.BTN_UP){
      if (millis() - debounce[3] < DebounceTime) continue;
      debounce[3] = millis();
      Serial.println("Button UP pressed");
      SetNextColor(3);
    }

    if (statusRegister.BTN_RIGHT){
      if (millis() - debounce[4] < DebounceTime) continue;
      debounce[4] = millis();
      Serial.println("Button RIGHT pressed");
      SetNextColor(4);
    }

    if (statusRegister.BTN_LEFT){
      if (millis() - debounce[5] < DebounceTime) continue;
      debounce[5] = millis();
      Serial.println("Button LEFT pressed");
      SetNextColor(5);
    }

    if (statusRegister.BTN_CENTER){
      if (millis() - debounce[6] < DebounceTime) continue;
      debounce[6] = millis();
      bright = bright + 5;
      if (bright > 31) bright = 5;
      leds.setBrightness(bright);
    }

    UpdateLedColor();
  }

  // enable new interrupt to this routine
  InHandleButton = false;
}

/**
 * loop through colors
 */
void SetNextColor(uint8_t i)
{
  if (LedStatus[i] < 9) LedStatus[i]++;
  else LedStatus[i] = 1;
}

/*
 * Display different colors for each leds
 * The colors are predefined in SFs_APA102.h
 */
void UpdateLedColor(){
  uint32_t col;

  for (uint8_t i = 0; i < totalLEDs; i++) {

    switch(LedStatus[i]) {
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
