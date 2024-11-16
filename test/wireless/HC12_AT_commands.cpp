//* Send AT commants to HC-12 module.

#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

const byte pinLed = LED_BUILTIN;
const byte pinSet = D1;
static const int RXPin = D5, TXPin = D7;
SoftwareSerial ss(RXPin, TXPin);

unsigned long msLastPrint = 0;

void bleja() { delay(100); }

void cmd(String cmd)
{
    Serial.println("Command \"" + cmd + "\" sent to HC-12...");
    ss.println(cmd);
    bleja();
    Serial.println(ss.readString());
    bleja();
}

void setup()
{
    WiFi.mode(WiFiMode::WIFI_OFF);
    WiFi.disconnect(true);
    pinMode(pinLed, OUTPUT);
    digitalWrite(pinLed, true);
    pinMode(pinSet, OUTPUT);
    digitalWrite(pinSet, LOW);

    Serial.begin(115200);
    Serial.println();
    ss.begin(9600);
    bleja();
    // cmd("AT+P4");
    cmd("AT+RX");
}

void loop()
{
    delay(10);
}
