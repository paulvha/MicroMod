/******************************************************************************
Based on the original library mentioned below, created a SparkFun Special function
to handle the button on the Micromod Input and Display carrier board.
https://www.sparkfun.com/products/16985

version 1.0 / March 2023 / Paulvha
======================================================================

registers.h
Fischer Moseley @ SparkFun Electronics
Original Creation Date: July 24, 2019

This file defines the virtual memory map on the Qwiic Button/Switch. The enum
provides a set of pointers for the various registers on the Qwiic
Button/Switch, and the unions wrap the various bits in the bitfield in an easy
to use uint8_t format

Development environment specifics:
	IDE: Arduino 1.8.9
	Hardware Platform: Arduino Uno/SparkFun Redboard
	Qwiic Button Version: 1.0.0
    Qwiic Switch Version: 1.0.0

This code is Lemonadeware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.
******************************************************************************/

//Register Pointer Map
enum MM_Qwiic_Button_Register : uint8_t
{
    SFE_QWIIC_BUTTON_ID = 0x00,
    SFE_QWIIC_BUTTON_FIRMWARE_MINOR = 0x01,
    SFE_QWIIC_BUTTON_FIRMWARE_MAJOR = 0x02,
    SFE_QWIIC_BUTTON_BUTTON_STATUS = 0x03,
    SFE_QWIIC_BUTTON_CLICKED_STATUS = 0x04,
    SFE_QWIIC_BUTTON_INTERRUPT_CONFIG = 0x05,
    SFE_QWIIC_BUTTON_BUTTON_DEBOUNCE_TIME = 0x06,   // 16 BITS
    SFE_QWIIC_BUTTON_I2C_ADDRESS = 0x8,
};

typedef union {
    struct
    {
        bool BTN_A: 1;          //0 Gets set to one if BTN_A is pushed
        bool BTN_B: 1;          //1 Gets set to one if BTN_B is pushed
        bool BTN_LEFT : 1;      //2 Gets set to one if BTN_LEFT is pushed
        bool BTN_RIGHT: 1;      //3 Gets set to one if BTN_RIGHT is pushed
        bool BTN_DOWN: 1;       //4 Gets set to one if BTN_DOWN is pushed
        bool BTN_UP: 1;         //5 Gets set to one if BTN_UP is pushed
        bool BTN_CENTER : 1;    //6 Gets set to one if BTN_Center is pushed
        bool isPressed : 1;     //7 Gets set to one if a button is pushed (EVENT_AVAILABLE) only the first read
    };
    uint8_t byteWrapped;
} MM_statusRegisterBitField;

typedef union {
  struct
  {
        bool BTN_A: 1;          //0 Gets set to once if BTN_A is released
        bool BTN_B: 1;          //1 Gets set to once if BTN_B is released
        bool BTN_LEFT : 1;      //2 Gets set to once if BTN_LEFT is released
        bool BTN_RIGHT: 1;      //3 Gets set to once if BTN_RIGHT is released
        bool BTN_DOWN: 1;       //4 Gets set to once if BTN_DOWN is released
        bool BTN_UP: 1;         //5 Gets set to once if BTN_UP is released
        bool BTN_Center : 1;    //6 Gets set to once if BTN_Center is released
        bool wasPressed : 1;    //7 Gets set to once if a button is released (EVENT_AVAILABLE)
   };
   uint8_t byteWrapped;
} MM_clickedRegisterBitField;

typedef union {
    struct
    {
        bool clickedEnable : 1; //This is bit 0. user mutable, set to 1 to enable an interrupt when the button is clicked. Defaults to 0.
        bool pressedEnable : 1; //user mutable, set to 1 to enable an interrupt when the button is pressed. Defaults to 0.
        bool : 6;
    };
    uint8_t byteWrapped;
} MM_interruptConfigBitField;

