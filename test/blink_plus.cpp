#include "Arduino.h"

const byte pinLed = LED_BUILTIN;
const byte pinOn = D6;
const byte pinVar = D7;
const byte pinOff = D8;

void setup()
{
  pinMode(pinLed, OUTPUT);
  pinMode(pinOn, OUTPUT);
  pinMode(pinVar, OUTPUT);
  pinMode(pinOff, OUTPUT);
  digitalWrite(pinLed, true);
  digitalWrite(pinOn, true);
  digitalWrite(pinOff, false);

  // while (true)
  //   delay(100);
}

bool val = false;

void loop()
{
  digitalWrite(pinLed, val = !val);
  digitalWrite(pinVar, val);
  delay(1000);
}
