/******************************************************************************
  Checks whether the button is pressed, and then prints its status over serial!
  created a SparkFun Special function to handle the button on the Micromod Input and output carrier board.
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
  
  A power off/on will be needed in that case to reset the Attiney84, as it does not use the reset-button

  This combined with an Attiny84 which does a cycle-stretching makes it a weak part of the board.
  
  Tested with MM Artemis, Teensy, nRF52840, ESP32, SAMD51
******************************************************************************/
#include <SFs_MM_Button.h>
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

void setup() {

  Serial.begin(115200);
  
#if (defined ARDUINO_TEENSY_MICROMOD)
  delay(500);
#else
  while (!Serial) {delay(10);}
#endif
  
  Serial.println("Example1 MicroMod print Buttons");
  Wire.begin(); //Join I2C bus

  // enable interrupt pin
  pinMode(INTPIN,INPUT);
  attachInterrupt(INTPIN, ButtonInt, FALLING);

  // check if button sensor will acknowledge over I2C
  if (button.begin() == false) {
    Serial.println("Device did not acknowledge! Freezing.");
    while(1);
  }

  // only enable pressed interrupt. Also setting Clicked interrupt will not work.
  // see top of sketch
  button.disableClickedInterrupt();

  // init debounce options
  button.setDebounceTime(0);
  for (uint8_t i = 0; i < NumButton; i++) debounce[i]= 0x0;
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
      // do something
    }

    if (statusRegister.BTN_B){
      if (millis() - debounce[1] < DebounceTime) continue;
      debounce[1] = millis();
      Serial.println("Button B pressed");
      // do something
    }

    if (statusRegister.BTN_DOWN){
      if (millis() - debounce[2] < DebounceTime) continue;
      debounce[2] = millis();
      Serial.println("Button DOWN pressed");
      // do something
    }

    if (statusRegister.BTN_UP){
      if (millis() - debounce[3] < DebounceTime) continue;
      debounce[3] = millis();
      Serial.println("Button UP pressed");
      // do something
    }

    if (statusRegister.BTN_RIGHT){
      if (millis() - debounce[4] < DebounceTime) continue;
      debounce[4] = millis();
      Serial.println("Button RIGHT pressed");
      // do something
    }

    if (statusRegister.BTN_LEFT){
      if (millis() - debounce[5] < DebounceTime) continue;
      debounce[5] = millis();
      Serial.println("Button LEFT pressed");
      // do something
    }

    if (statusRegister.BTN_CENTER){
      if (millis() - debounce[6] < DebounceTime) continue;
      debounce[6] = millis();
      Serial.println("Button CENTER pressed");
      // do something
    }
  }

  // enable new interrupt to this routine
  InHandleButton = false;
}
