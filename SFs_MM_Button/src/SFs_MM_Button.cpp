/******************************************************************************
Based on the original library mentioned below, created a SparkFun Special function
to handle the button on the Micromod Input and Display carrier board.
https://www.sparkfun.com/products/16985

version 1.0 / March 2023 / Paulvha
======================================================================

SparkFun_Qwiic_Button.cpp
SparkFun Qwiic Button/Switch Library Source File
Fischer Moseley @ SparkFun Electronics
Original Creation Date: July 24, 2019

This file implements the QwiicButton class, prototyped in SparkFun_Qwiic_Button.h

Development environment specifics:
	IDE: Arduino 1.8.9
	Hardware Platform: Arduino Uno/SparkFun Redboard
	Qwiic Button Version: 1.0.0
  Qwiic Switch Version: 1.0.0

This code is Lemonadeware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.
******************************************************************************/

#include <Wire.h>
#include <SFs_MM_Button.h>


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/*-------------------------------- Device Status ------------------------*/

bool MM_QwiicButton::begin(uint8_t address, TwoWire &wirePort)
{
    _deviceAddress = address; //grab the address that the sensor is on
    _i2cPort = &wirePort;     //grab the port that the user wants to use

    //return true if the device is connected and the device ID is what we expect
    return (isConnected() && checkDeviceID());
}

bool MM_QwiicButton::isConnected()
{
    _i2cPort->beginTransmission(_deviceAddress);
    if (_i2cPort->endTransmission() == 0)
        return true;
    return false;
}

uint8_t MM_QwiicButton::deviceID()
{
    return readSingleRegister(SFE_QWIIC_BUTTON_ID); //read and return the value in the ID register
}

bool MM_QwiicButton::checkDeviceID()
{
    return (deviceID() == SFE_QWIIC_BUTTON_DEV_ID); //Return true if the device ID matches
}

uint8_t MM_QwiicButton::getDeviceType()
{
    if (isConnected())
    {   //only try to get the device ID if the device will acknowledge
        uint8_t id = deviceID();
        if (id == SFE_QWIIC_BUTTON_DEV_ID)
            return 1;
    }
    return 0;
}

uint16_t MM_QwiicButton::getFirmwareVersion()
{
    uint16_t version = (readSingleRegister(SFE_QWIIC_BUTTON_FIRMWARE_MAJOR)) << 8;
    version |= readSingleRegister(SFE_QWIIC_BUTTON_FIRMWARE_MINOR);
    return version;
}

bool MM_QwiicButton::setI2Caddress(uint8_t address)
{
    if (address < 0x08 || address > 0x77)
    {
        Serial.println("Error1");
        return 1; //error immediately if the address is out of legal range
    }

    bool success = writeSingleRegister(SFE_QWIIC_BUTTON_I2C_ADDRESS, address);

    if (success == true)
    {
        _deviceAddress = address;
        return true;
    }

    else
    {
        Serial.println("Error2");
        return false;
    }
}

uint8_t MM_QwiicButton::getI2Caddress()
{
    return _deviceAddress;
}

/*------------------------------ Buzzer ---------------------- */
void MM_QwiicButton::buzzerBegin()
{
    pinMode(BUZZER,OUTPUT);
}

void MM_QwiicButton::buzzer(uint32_t freq, uint32_t duration)
{
    tone(BUZZER, freq, duration);
}

/*------------------------------ Button Status ---------------------- */
bool MM_QwiicButton::isPressed()
{
    MM_statusRegisterBitField statusRegister;
    statusRegister.byteWrapped = readSingleRegister(SFE_QWIIC_BUTTON_BUTTON_STATUS);
    return statusRegister.isPressed;
}

bool MM_QwiicButton::isA_Pressed()
{
    MM_statusRegisterBitField statusRegister;
    statusRegister.byteWrapped = readSingleRegister(SFE_QWIIC_BUTTON_BUTTON_STATUS);
    return statusRegister.BTN_A;
}

bool MM_QwiicButton::isB_Pressed()
{
    MM_statusRegisterBitField statusRegister;
    statusRegister.byteWrapped = readSingleRegister(SFE_QWIIC_BUTTON_BUTTON_STATUS);
    return statusRegister.BTN_B;
}

bool MM_QwiicButton::isUp_Pressed()
{
    MM_statusRegisterBitField statusRegister;
    statusRegister.byteWrapped = readSingleRegister(SFE_QWIIC_BUTTON_BUTTON_STATUS);
    return statusRegister.BTN_UP;
}

bool MM_QwiicButton::isDown_Pressed()
{
    MM_statusRegisterBitField statusRegister;
    statusRegister.byteWrapped = readSingleRegister(SFE_QWIIC_BUTTON_BUTTON_STATUS);
    return statusRegister.BTN_DOWN;
}

bool MM_QwiicButton::isRight_Pressed()
{
    MM_statusRegisterBitField statusRegister;
    statusRegister.byteWrapped = readSingleRegister(SFE_QWIIC_BUTTON_BUTTON_STATUS);
    return statusRegister.BTN_RIGHT;
}

bool MM_QwiicButton::isCenter_Pressed()
{
    MM_statusRegisterBitField statusRegister;
    statusRegister.byteWrapped = readSingleRegister(SFE_QWIIC_BUTTON_BUTTON_STATUS);
    return statusRegister.BTN_CENTER;
}

uint8_t MM_QwiicButton::getStatusRegister()
{
    return(readSingleRegister(SFE_QWIIC_BUTTON_BUTTON_STATUS));
}

uint16_t MM_QwiicButton::getDebounceTime()
{
    return readDoubleRegister(SFE_QWIIC_BUTTON_BUTTON_DEBOUNCE_TIME);
}

uint8_t MM_QwiicButton::setDebounceTime(uint16_t time)
{
    return writeDoubleRegisterWithReadback(SFE_QWIIC_BUTTON_BUTTON_DEBOUNCE_TIME, time);
}

/*------------------- Interrupt Status/Configuration ---------------- */
uint8_t MM_QwiicButton::enablePressedInterrupt()
{
    MM_interruptConfigBitField interruptConfigure;
    interruptConfigure.byteWrapped = readSingleRegister(SFE_QWIIC_BUTTON_INTERRUPT_CONFIG);
    interruptConfigure.pressedEnable = 1;
    return writeSingleRegisterWithReadback(SFE_QWIIC_BUTTON_INTERRUPT_CONFIG, interruptConfigure.byteWrapped);
}

uint8_t MM_QwiicButton::disablePressedInterrupt()
{
    MM_interruptConfigBitField interruptConfigure;
    interruptConfigure.byteWrapped = readSingleRegister(SFE_QWIIC_BUTTON_INTERRUPT_CONFIG);
    interruptConfigure.pressedEnable = 0;
    return writeSingleRegisterWithReadback(SFE_QWIIC_BUTTON_INTERRUPT_CONFIG, interruptConfigure.byteWrapped);
}

uint8_t MM_QwiicButton::enableClickedInterrupt()
{
    MM_interruptConfigBitField interruptConfigure;
    interruptConfigure.byteWrapped = readSingleRegister(SFE_QWIIC_BUTTON_INTERRUPT_CONFIG);
    interruptConfigure.clickedEnable = 1;
    return writeSingleRegisterWithReadback(SFE_QWIIC_BUTTON_INTERRUPT_CONFIG, interruptConfigure.byteWrapped);
}

uint8_t MM_QwiicButton::disableClickedInterrupt()
{
    MM_interruptConfigBitField interruptConfigure;
    interruptConfigure.byteWrapped = readSingleRegister(SFE_QWIIC_BUTTON_INTERRUPT_CONFIG);
    interruptConfigure.clickedEnable = 0;
    return writeSingleRegisterWithReadback(SFE_QWIIC_BUTTON_INTERRUPT_CONFIG, interruptConfigure.byteWrapped);
}

uint8_t MM_QwiicButton::resetInterruptConfig()
{
    MM_interruptConfigBitField interruptConfigure;
    interruptConfigure.pressedEnable = 1;
    interruptConfigure.clickedEnable = 1;
    return writeSingleRegisterWithReadback(SFE_QWIIC_BUTTON_INTERRUPT_CONFIG, interruptConfigure.byteWrapped);
}

/*------------------------- Internal I2C Abstraction ---------------- */

uint8_t MM_QwiicButton::readSingleRegister(MM_Qwiic_Button_Register reg)
{
    _i2cPort->beginTransmission(_deviceAddress);
    _i2cPort->write(reg);
    _i2cPort->endTransmission();
    delay(20); //paulvha due to the pesty Attiny84 the RF52840 got crazy
    //typecasting the 1 parameter in requestFrom so that the compiler
    //doesn't give us a warning about multiple candidates
    if (_i2cPort->requestFrom(_deviceAddress, static_cast<uint8_t>(1)) != 0)
    {
        return _i2cPort->read();
    }
    return 0;
}

uint16_t MM_QwiicButton::readDoubleRegister(MM_Qwiic_Button_Register reg)
{ //little endian
    _i2cPort->beginTransmission(_deviceAddress);
    _i2cPort->write(reg);
    _i2cPort->endTransmission();
    delay(20); //paulvha due to the pesty Attiny84 the RF52840 got crazy
    //typecasting the 2 parameter in requestFrom so that the compiler
    //doesn't give us a warning about multiple candidates
    if (_i2cPort->requestFrom(_deviceAddress, static_cast<uint8_t>(2)) != 0)
    {
        uint16_t data = _i2cPort->read();
        data |= (_i2cPort->read() << 8);
        return data;
    }
    return 0;
}

bool MM_QwiicButton::writeSingleRegister(MM_Qwiic_Button_Register reg, uint8_t data)
{
    _i2cPort->beginTransmission(_deviceAddress);
    _i2cPort->write(reg);
    _i2cPort->write(data);
    if (_i2cPort->endTransmission() == 0)
        return true;
    return false;
}

bool MM_QwiicButton::writeDoubleRegister(MM_Qwiic_Button_Register reg, uint16_t data)
{
    _i2cPort->beginTransmission(_deviceAddress);
    _i2cPort->write(reg);
    _i2cPort->write(lowByte(data));
    _i2cPort->write(highByte(data));{
    if (_i2cPort->endTransmission() == 0)
        return true;
    }
    return false;
}

uint8_t MM_QwiicButton::writeSingleRegisterWithReadback(MM_Qwiic_Button_Register reg, uint8_t data)
{
    if (writeSingleRegister(reg, data))
        return 1;
    if (readSingleRegister(reg) != data)
        return 2;
    return 0;
}

uint16_t MM_QwiicButton::writeDoubleRegisterWithReadback(MM_Qwiic_Button_Register reg, uint16_t data)
{
    if (writeDoubleRegister(reg, data))
        return 1;
    if (readDoubleRegister(reg) != data)
        return 2;
    return 0;
}
