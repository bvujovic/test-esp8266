#include "Arduino.h"

const byte pinLed = LED_BUILTIN;  // Za ESP07 LED_BUILTIN bi trebalo da bude 2
const byte pinOn = 12;  // D6 = 12
const byte pinVar = 13; // D7 = 13
const byte pinOff = 15; // D8 = 15

void setup()
{
  pinMode(pinLed, OUTPUT);
  pinMode(pinOn, OUTPUT);
  pinMode(pinVar, OUTPUT);
  pinMode(pinOff, OUTPUT);
  digitalWrite(pinLed, true);
  digitalWrite(pinOn, true);
  digitalWrite(pinOff, false);
}

bool val = false;

void loop()
{
  digitalWrite(pinLed, val);
  digitalWrite(pinVar, val);
  val = !val;
  delay(1000);
}
