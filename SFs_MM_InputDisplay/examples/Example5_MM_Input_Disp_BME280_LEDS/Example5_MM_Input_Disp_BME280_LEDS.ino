/*  
  Tested for the Sparkfun MicroMod Input and Display
  DEV-16985 (https://www.sparkfun.com/products/16985)

  Will show BME280 results on display with splitscreen and leds
  3 leds, Pressure, Humidity or temperature are green when within pre-defined bounderies
  else they will show red. 

 depends on https://github.com/paulvha/SF_APA102
 depends on https://github.com/adafruit/Adafruit-GFX-Library
 depends on https://github.com/adafruit/Adafruit_ILI9341
 depends on https://github.com/adafruit/Adafruit_BusIO
  
  Version 1.0 / march 2023 / Paulvha
  =================================================================
 *
 *                                            split_vertical  split_horizontal   SPLIT_BOTH_BELOW      SPLIT_BOTH_ABOVE 
 * define 1 screen     define 4 screens       2 or 3 screens  2 or 3 screens      3 screens only        3 screens only    
 *                          1  |  2              1 | 2 | 3            1              2  |  3               1
 *     1                    ---|----               |   |           -------           -------            --------
 *                          3  |  4                |   |              2                 1                2 | 3
 *                                                                 -------  
 * valid options for ROTATION                                         3  
 *
  ROTATION_PORTRAIT       // w= 240 , H = 320
  ROTATION_LANDSCAPE      // w= 320 , h = 240
  ROTATION_REV_PORTRAIT   // w= 240 , H = 320 upside
  ROTATION_REV_LANDSCAPE  // w= 320 , h = 240 upside
 *
 * valid options for SplitScreen
 *
  SPLIT_HORIZONTAL       // in case of 1, 2 or 3 screens
  SPLIT_VERTICAL         // in case of 1, 2 or 3 screens
  SPLIT_BOTH_BELOW       // ONLY in case of 3 screens: Screen 1 at the bottom left to right / screen 2 and 3 are next to each other above
  SPLIT_BOTH_ABOVE       // ONLY in case of 3 screens: Screen 1 at the top  left to right / screen 2 and 3 are next to each other below

  BME280
  ====================================================================
  This has been tested with an Adafruit BME280.

   BME280       QWICC (MM_input_display)
    VIN 
    3V3         3v3
    GND         GND
    SCK         SCL
    SDO
    SDI         SDA
    CS

  OR depending on your BME280 board

  Parts of the code below are coming from
  Sparkfun BME280 library: https://github.com/sparkfun/SparkFun_BME280_Arduino_Library
  The expected I2C address is 0x77.

    BME280      QWIIC
    GND         GND
    VCC         3V3
    SCK         SCL
    SDI         SDA
*/

#include <Wire.h>
#include "SparkFunBME280.h"
#include "MM_InputDisplay.h"  // includes SFs_APA102.h for leds

///////////// DEFINE SPLITSCREEN SETTINGS //////
#define NUMSCREENS   2
#define SPLITSCREEN  SPLIT_HORIZONTAL
#define ROTATION     ROTATION_LANDSCAPE

/////////////// BME280 /////////////////////////
float m_temperature;
// sent temperature in Celsius (1) or Fahrenheit (0)
#define TempInCelsius  1
#define Temp_HIGH  25     // set for Celsius or Fahrenheit degrees
#define Temp_LOW   17     // set for Celsius or Fahrenheit degrees

float m_humidity;
#define Hum_HIGH   60     // %to stuffy 
#define Hum_LOW    40     // %to dry 

float m_pressure;
#define Pres_HIGH   1020   // hPa pff too high pressing
#define Pres_LOW    1000   // hPa oh oh storm is on its way

float m_altitude;
// sent altitude in meters (1) or feet (0)
#define AltitudeInMeter  1

// sets local pressure on sealevel to calculate altitude correctly
#define PressureSeaLevel   101325.0

// set address
#define I2C_address   0x77  
////////////////// display //////////////////////
int16_t Tx,Ty,Hx,Hy,Px,Py,Ax,Ay;

/////////////// constructors /////////////////////
// construct for BME280 sensor
BME280 mySensor;

// Construct leds-object
SFs_APA102 leds(totalLEDs, dataPin, clockPin);

// constructor for MicroMod display
SFs_MM MM;


void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Example5_MMID_BME280_leds");

  //>>>>>>>>> setup APA102 leds <<<<<<<<<<<<<
  leds.begin();
  
  // initalize all the leds with the same color
  leds.fill(color_black, 0, totalLEDs);
  
  leds.setBrightness(3);    // max 31
  leds.setIntensity(50);    // max 100
  
  // update leds
  leds.show();

  // >>>>>>>>> setup screen <<<<<<<<<<<<<<<<<<
  MM.begin();

  // set backlight display
  MM.setBacklight(255);

  // init split screens
  MM.InitSplScreen(NUMSCREENS,SPLITSCREEN,ROTATION);

  // screen 0, textsize 2 ( * 6x8) = 12x18), text, store current X-location  (true)
  MM.SetTextMiddle(0,2,"Example5\n\n",true);
  MM.AddTextMiddle(0,2,"MMID BME280 LEDS\n",true);

  // >>>>>>>>> init BME280 <<<<<<<<<<<<<<<<<<
  Wire.begin();

  mySensor.setI2CAddress(I2C_address);
  
  if (mySensor.beginI2C() == false) // Begin communication over I2C
  {
    Serial.println(F("The BME280 did not respond. Please check wiring. freeze \r"));
    MM.SetTextColor(0,ILI9341_RED);
    MM.AddTextMiddle(0,2,"Error BME280 => freeze\n",true);
    while(1);
  }

  // sets local pressure on sealevel to calculate altitude correctly
  mySensor.setReferencePressure(PressureSeaLevel);
  
  //>>>>>>>>> set initial BME280 results display <<<<<<<<<<<<<<<<<<
  
  MM.SetText(1,2," Temperature:", true);
  Tx = MM.GetCurrentx(1);  // get current x from screen 1
  Ty = MM.GetCurrenty(1);  // get current Y from screen 1

  MM.AddText(1,2,"\n Humidity:   ", true);
  Hx = MM.GetCurrentx(1);  // get current x from screen 1
  Hy = MM.GetCurrenty(1);  // get current Y from screen 1

  MM.AddText(1,2,"\n Pressure:   ", true);
  Px = MM.GetCurrentx(1);  // get current x from screen 1
  Py = MM.GetCurrenty(1);  // get current Y from screen 1

  MM.AddText(1,2,"\n Altitude:   ", true);
  Ax = MM.GetCurrentx(1);  // get current x from screen 1
  Ay = MM.GetCurrenty(1);  // get current Y from screen 1

  // set headers above leds
  MM.AddText(1,2,"\n\n            Press Hum Tmp", true);
}

void loop() {
  char p[10];
  uint32_t Tcol, Hcol, Pcol;

  // get BME280 values
  if (TempInCelsius) m_temperature = mySensor.readTempC();
  else m_temperature = mySensor.readTempF();
  
  m_humidity = mySensor.readFloatHumidity();
  m_pressure = mySensor.readFloatPressure() / 100;

  if (AltitudeInMeter) m_altitude = mySensor.readFloatAltitudeMeters();
  else m_altitude = mySensor.readFloatAltitudeFeet();

  // handle temperature to display
  if (TempInCelsius) sprintf(p,"%2.2f C", m_temperature);
  else sprintf(p,"%2.2f F", m_temperature);
  
  if (m_temperature > Temp_HIGH) Tcol = color_red;
  else if (m_temperature < Temp_LOW) Tcol = color_blue;
  else Tcol = color_green;

  // screen 1, textsize 2 ( * 6x8) = 12x18), text, saved-X,saved-Y, # digits to clear, store current X-location  (true)
  MM.clrSetPos(1, 2, p, Tx, Ty, 7, true);

  // handle humidity to display
  sprintf(p,"%2.2f %%", m_humidity);
  // screen 1, textsize 2 ( * 6x8) = 12x18), text, saved-X,saved-Y, # digits to clear, store current X-location  (true)

  if (m_humidity > Hum_HIGH) Hcol = color_red;
  else if (m_humidity < Hum_LOW) Hcol = color_blue;
  else Hcol = color_green;

  MM.clrSetPos(1, 2, p, Hx, Hy, 7, true);  

  // handle pressure to display
  sprintf(p,"%2.2f hPa", m_pressure);
  // screen 1, textsize 2 ( * 6x8) = 12x18), text, saved-X,saved-Y, # digits to clear, store current X-location  (true)
  MM.clrSetPos(1, 2, p, Px, Py, 10, true);  

  if(m_pressure > Pres_HIGH) Pcol = color_red;
  else if (m_pressure < Pres_LOW) Pcol = color_purple;
  else Pcol = color_green;

  // handle Altitude to display
  if (AltitudeInMeter) sprintf(p,"%2.2f M", m_altitude);
  else sprintf(p,"%2.2f F", m_altitude);
  // screen 1, textsize 2 ( * 6x8) = 12x18), text, saved-X, saved-Y, # digits to clear, store current X-location  (true)
  MM.clrSetPos(1, 2, p, Ax, Ay, 10, true);  

  // set leds
  leds.setPixelColor(0, Tcol);
  leds.setPixelColor(1, Hcol);
  leds.setPixelColor(2, Pcol);
 
  // show new setting
  leds.show();
  
  delay(1000);
}
