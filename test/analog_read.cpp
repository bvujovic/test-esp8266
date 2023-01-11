#include <Arduino.h>

const int pin = A0;

void setup()
{
    Serial.begin(115200);
    pinMode(pin, INPUT);
}

void loop()
{
    Serial.println(analogRead(pin));
    delay(2000);
}