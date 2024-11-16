#include <Arduino.h>

const byte pinLed = LED_BUILTIN;
const byte pinSet = D3;

void setup()
{
    Serial.begin(9600); // Serial port to HC12 (No serial monitor - HC uses RX & TX)
    pinMode(pinLed, OUTPUT);
    digitalWrite(pinLed, true);
}

void loop()
{
    while (Serial.available())
    {
        Serial.readString();
        digitalWrite(pinLed, false);
        delay(250);
        digitalWrite(pinLed, true);
    }
}