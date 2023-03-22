/******************************************************************************
Based on the original library mentioned below, created a SparkFun Special function
to handle the button on the Micromod Input and Display carrier board.
https://www.sparkfun.com/products/16985

version 1.0 / March 2023 / Paulvha
======================================================================

SparkFun_Qwiic_Button.h
SparkFun Qwiic Button/Switch Library Header File
Fischer Moseley @ SparkFun Electronics
Original Creation Date: July 24, 2019
https://github.com/sparkfunX/

This file prototypes the QwiicButton class, implemented in SparkFun_Qwiic_Button.cpp.

Development environment specifics:
	IDE: Arduino 1.8.9
	Hardware Platform: Arduino Uno/SparkFun Redboard
	Qwiic Button Breakout Version: 1.0.0
    Qwiic Switch Breakout Version: 1.0.0

This code is Lemonadeware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.
******************************************************************************/

#ifndef __SFs_MM_Button_H__
#define __SFs_MM_Button_H__

#include <Wire.h>
#include <Arduino.h>
#include "SFs_MM_registers.h"
#include "SFs_MM_Buzzer.h"

#define SFE_QWIIC_BUTTON_DEFAULT_ADDRESS  0x71 // default MM I2C address of the button
#define SFE_QWIIC_BUTTON_DEV_ID  0xAE          // device ID of the MM Qwiic Button

class MM_QwiicButton
{
private:
    TwoWire *_i2cPort;                      //Generic connection to user's chosen I2C port
    uint8_t _deviceAddress;                 //I2C address of the button/switch
    uint8_t deviceID();                     //Return the 8-bit device ID of the attached device.
    bool checkDeviceID();                   //Returns true if the device ID matches that of either the button or the switch

public:
    //Device status
    bool begin(uint8_t address = SFE_QWIIC_BUTTON_DEFAULT_ADDRESS, TwoWire &wirePort = Wire);
    uint8_t getDeviceType();                //Returns device_ID()
    uint16_t getFirmwareVersion();          //Returns the firmware version of the attached device as a 16-bit integer.
    bool setI2Caddress(uint8_t address);    //Configures the attached device to attach to the I2C bus using the specified address
    uint8_t getI2Caddress();                //Returns the I2C address of the device.
    bool isConnected();                     //Returns true if the button/switch will acknowledge over I2C, and false otherwise

    //Button status/config
    bool isPressed();                       //Returns 1 if a button/switch is pressed, and 0 otherwise

    uint16_t getDebounceTime();             //Returns the time that the button waits for the mechanical contacts to settle, (in milliseconds).
    uint8_t setDebounceTime(uint16_t time); //Sets the time that the button waits for the mechanical contacts to settle (in milliseconds) and checks
                                            // if the register was set properly. Returns 0 on success, 1 on register I2C write fail, and 2 if the
                                            // value didn't get written into the register properly.

    bool isRight_Pressed();                 //get true is the a button is still pressed
    bool isLeft_Pressed();                  // be aware that debounce is now handled by the Attiny84 on the board
    bool isUp_Pressed();                    // and there is an issue with that
    bool isDown_Pressed();                  // https://github.com/sparkfun/MicroMod_Input_and_Display_Carrier/issues
    bool isA_Pressed();                     // see example 1 for alternative approach
    bool isB_Pressed();
    bool isCenter_Pressed();
    uint8_t getStatusRegister();            //get info about ALL the buttons being pressed (or not)

    //Interrupt status/config
    uint8_t enablePressedInterrupt();  //When called, the interrupt will be configured to trigger when the button is pressed. If
                                       // enableClickedInterrupt() has also been called, then the interrupt will trigger on either a push or a click.
    uint8_t disablePressedInterrupt(); //When called, the interrupt will no longer be configured to trigger when the button is pressed. If
                                       // enableClickedInterrupt() has also been called, then the interrupt will still trigger on the button click.
    uint8_t enableClickedInterrupt();  //When called, the interrupt will be configured to trigger when the button is clicked. If
                                       // enablePressedInterrupt() has also been called, then the interrupt will trigger on either a push or a click.
    uint8_t disableClickedInterrupt(); //When called, the interrupt will no longer be configured to trigger when the button is clicked. If
                                       // enablePressedInterrupt() has also been called, then the interrupt will still trigger on the button press.
    uint8_t resetInterruptConfig();    //Resets the interrupt configuration back to defaults. (both pressed and clicked enabled)


    // Buzzer setting
    void buzzerBegin();                            // call first to enable output
    void buzzer(uint32_t freq, uint32_t duration); // freq are defined in SFs_MM_Buzzer.h

    //Internal I2C Abstraction
    uint8_t readSingleRegister(MM_Qwiic_Button_Register reg);           //Reads a single 8-bit register.
    uint16_t readDoubleRegister(MM_Qwiic_Button_Register reg);          //Reads a 16-bit register (little endian).
    bool writeSingleRegister(MM_Qwiic_Button_Register reg, uint8_t data);  //Attempts to write data into a single 8-bit register. Does not check to make
                                                                        // sure it was written successfully. Returns 0 if there wasn't an error on
                                                                        // I2C transmission, and 1 otherwise.
    bool writeDoubleRegister(MM_Qwiic_Button_Register reg, uint16_t data); //Attempts to write data into a double (two 8-bit) registers. Does not check
                                                                        // to make sure it was written successfully. Returns 0 if there wasn't an
                                                                        // error on I2C transmission, and 1 otherwise.
    uint8_t writeSingleRegisterWithReadback(MM_Qwiic_Button_Register reg, uint8_t data); //Writes data into a single 8-bit register, and checks to
                                                                        // make sure that the data was written successfully. Returns 0 on no error,
                                                                        // 1 on I2C write fail, and 2 if the register doesn't read back the same
                                                                        // value that was written.
    uint16_t writeDoubleRegisterWithReadback(MM_Qwiic_Button_Register reg, uint16_t data); //Writes data into a double (two 8-bit) registers, and checks
                                                                        // to make sure that the data was written successfully. Returns 0 on no
                                                                        // error, 1 on I2C write fail, and 2 if the register doesn't read back the
                                                                        // same value that was written.
};
#endif // __SparkFun_MM_Qwiic_Button_H__
