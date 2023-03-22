/******************************************************************************
  This sketch is demonstrating the usage of the buzzer on the Micromod Input and Display carrier board.
  It can play pre-defined songs (as stored in the attached songs.h) but also your own
  defined tune (see wissle below)
  Parts are taken from :
  http://www.arduino.cc/en/Tutorial/Tone
  https://www.hibit.dev/posts/62/playing-popular-songs-with-arduino-and-a-buzzer

  Version 1.0 / March 2023 / paulvha

  Hardware Connections:
  Print it to the serial monitor at 115200 baud.

  Distributed as-is; no warranty is given.

  Tested on Micromod Artemis(*2), ESP32 (*1), nRF52840, SAMD51, Teensy
  (*1) Make sure to use the ESP32-Arduino library. The Sparkfun ESP32 does not seem to have a tone()
  (*2) The songs are played slower with Artemis.
******************************************************************************/

#include "SFs_MM_Button.h"
#include "songs.h"

MM_QwiicButton button;
void setup() {
  Serial.begin(115200);
#if (defined ARDUINO_TEENSY_MICROMOD)   // the Serial test fails on Teensy
  delay(500);
#else
  while (!Serial) {delay(10);}
#endif

  Serial.println("Example5 MicroMod Buzzer demo");

  // this will enable the output pin
  button.buzzerBegin();

  displaySongs();
}

void loop() {
  Serialflush();

  Serial.println("Provide your selection to play");

  //Wait for serial data to be available
  while (Serial.available() == 0);
  delay(100);
  if (Serial.available()) {
    String stringBuffer = Serial.readString();
    char charBuffer[10];
    int sel = 0;
    stringBuffer.toCharArray(charBuffer, 10);
    sscanf(charBuffer, "%d", &sel);

    switch (sel) {
      case 0:
        displaySongs();
        break;
      case 1:
        play(bye_melody,bye_durations,bye_length);
        break;
      case 2:
        play(Mb_melody,Mb_durations,Mb_length);
        break;
      case 3:
        play(Pp_melody,Pp_durations,Pp_length);
        break;
      case 4:
        wissle();
        break;
      case 5:
        play(No_melody,No_durations,No_length);
        break;
      case 6:
        play(Hp_melody,Hp_durations,Hp_length);
        break;
      case 7:
        play(Mc_melody,Mc_durations,Mc_length);
        break;
      case 8:
        play(Ls_melody,Ls_durations,Ls_length);
        break;
      case 9:
        play(Es_melody,Es_durations,Es_length);
        break;
      case 10:
        play(Sw_melody,Sw_durations,Sw_length);
        break;
      case 11:
        play(La_melody,La_durations,La_length);
        break;
      default:
        Serial.print("Invalid selection ");
        Serial.println(sel);
        break;
    } // case
  } // serial.available
}

void displaySongs(){
  Serial.println();
  Serial.println ("1\tbye");
  Serial.println ("2\tMario Bros");
  Serial.println ("3\tPink Panther");
  Serial.println ("4\tWissle");
  Serial.println ("5\tNokia ringtone");
  Serial.println ("6\tHappy Birthday");
  Serial.println ("7\tMerry Christmas");
  Serial.println ("8\tThe Lion sleeps tonight");
  Serial.println ("9\tEd Sheeran - Shape of you");
  Serial.println ("10\tStarWars");
  Serial.println ("11\tEasy piano");
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
 * play predefined songs
 * Source:  http://www.arduino.cc/en/Tutorial/Tone
 */
void play(int *melody, int *duration, uint16_t ml)
{
  Serialflush();
  Serial.println("Press <enter> to stop playing");

  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < ml; thisNote++) {

    if (Serial.available()) return;   // stop playing

    // to calculate the note duration, take one second divided by the note type.

    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.

    int noteDuration = 1000 / duration[thisNote];

    // if REST Teensy is not good at handling that
    if (melody[thisNote] == REST){
      #if (defined ARDUINO_TEENSY_MICROMOD)
        // -2 for function overhead
        delay(noteDuration - 2);
      #else
        button.buzzer(melody[thisNote], noteDuration);
      #endif
    }
    else
      button.buzzer(melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.

    // the note's duration + 30% seems to work well:

    int pauseBetweenNotes = noteDuration * 1.30;

    delay(pauseBetweenNotes);
  }
}

/**
 * Use your own tune on the buzzer
 */
void wissle()
{
  button.buzzer( NOTE_C7, 150); //C
  delay(150);
  button.buzzer( NOTE_D7, 150); //D
  delay(150);
  button.buzzer( NOTE_E7, 150); //E
  delay(150);
}
