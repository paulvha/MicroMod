/******************************************************************************
  A configurator for changing the I2C address on the MM_Button that walks
  the user through finding the address of their button, and then changing it!

  Fischer Moseley @ SparkFun Electronics
  Original Creation Date: July 30, 2019

  This code is Lemonadeware; if you see me (or any other SparkFun employee) at the
  local, and you've found our code helpful, please buy us a round!

  Hardware Connections:
  Print it to the serial monitor at 115200 baud.

  Distributed as-is; no warranty is given.

  ************ update Mach 2023 / paulvha ******************************
  Adjusted for the Micromod input output carrier board.
  
  Tested on Micromod Artemis, ESP32, nRF52840, samd51, Teensy
  Given the Attiny84 an Artemis processor has extra read added

  !!!!!!!!!!!!!!!!!!!!!!!  IMPORTANT TO KNOW  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  Unlike the example code on:
  https://github.com/sparkfun/MicroMod_Input_and_Display_Carrier/tree/master/Firmware/Input_and_Display_Button_Master
  suggests, there is never an update to EEPROM on the Attiny84 happening. 
  All is only changed in RAM and thus a change in I2C address is NOT saved after reset.
  
  Actually making this program a "bit" unneccesary.. but at least you know now. :-)

******************************************************************************/

#include <SFs_MM_Button.h>
MM_QwiicButton button;

// set to 1 to display Attiny84 registers before and after change
#define SHOWREG 0

void setup() {
  Serial.begin(115200);
#if (defined ARDUINO_TEENSY_MICROMOD)
  delay(500);
#else
  while (!Serial) {delay(10);}
#endif

  Serial.println("Example2 MM button I2C change");
  Wire.begin(); //Join I2C bus

  //check if button will acknowledge over I2C
  if (button.begin() == false) {
    Serial.println("Device did not acknowledge! Running scanner.");
  }
  else{
    Serial.println("Device acknowledged!");

#if SHOWREG
    // debug only 8 registers are used
    for(uint8_t i =0; i < 0x9 ;i++) {
      Serial.print("Reg ");
      Serial.print(i, HEX); 
      Serial.print(" val 0x");
      Serial.println(button.readSingleRegister((MM_Qwiic_Button_Register) i) ,HEX);
    }
#endif
  } 
  
    Serial.println();
    Serial.println("Enter a new I2C address for the MM onboard Button to use!");
    Serial.println("Don't use the 0x prefix. For instance, if you wanted to");
    Serial.println("change the address to 0x5B, you would enter 5B and press enter.");
    Serial.println();
    Serial.println("One more thing! Make sure your line ending is set to 'Both NL & CR'");
    Serial.println("in the Serial Monitor.");
    Serial.println();

    //Wait for serial data to be available
    while (Serial.available() == 0);
  
    if (Serial.available()) {
      uint8_t newAddress = 0;
      String stringBuffer = Serial.readString();
      char charBuffer[10];
      stringBuffer.toCharArray(charBuffer, 10);
      uint8_t success = sscanf(charBuffer, "%x", &newAddress);
  
      if (success) {
        if (newAddress > 0x08 && newAddress < 0x77) {
          Serial.println("Character received, and device address is valid!");
          Serial.print("Attempting to set device address to 0x");
          Serial.println(newAddress, HEX);
  
          if (button.setI2Caddress(newAddress) == true) {
            Serial.println("Device address set succeeded!");
#if SHOWREG
          // debug only 8 registers are used
          for (uint8_t i =0; i < 0x9 ;i++) {
            Serial.print("Reg ");
            Serial.print(i, HEX); 
            Serial.print(" val 0x");
            Serial.println(button.readSingleRegister( (MM_Qwiic_Button_Register) i),HEX);
          }
#endif
          }
          else {
            Serial.println("Device address set failed!");
          }
  
          delay(100); //give the hardware time to do whatever configuration it needs to do
  
          if (button.isConnected()) {
            Serial.println("Device will acknowledge on new I2C address!");
          }
          else {
            Serial.println("Device will not acknowledge on new I2C address.");
          }
        }
  
        else {
          Serial.println("Address out of range! Try an adress between 0x08 and 0x77");
        }
      }
  
      else {
        Serial.print("Invalid Text! Try again.");
      }
    }
    delay(100);
}


void loop() {
  //if no I2C device found or on-board button correctly set to new address, 
  //scan for available I2C devices
  uint8_t error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 0x1; address < 0x7F; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmission to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    
#if (defined ARDUINO_AM_AP3_SFE_ARTEMIS_MICROMOD)  || (defined ARDUINO_APOLLO3_SFE_ARTEMIS_MM_PB)   // V1 and V2 definition
    // A read is added as the Artemis processor was not handling endTransmission() correct on this board
    // The Attiny84 is suspected doing a long clock-cycle-delay
    // On Artemis try to do an extra read of one byte
    error = Wire.requestFrom(address, static_cast<uint8_t>(1));
    if (error == 1) error = 0;  // we are good
    else error = 2; // NOT good
#endif 

    if (error == 0 )
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  delay(5000);           // wait 5 seconds for next scan
}
