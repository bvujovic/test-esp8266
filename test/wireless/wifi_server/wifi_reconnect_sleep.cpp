
#include <Arduino.h>
typedef unsigned long ulong;

#include <WiFiServerBasics.h>

ESP8266WebServer server(80);
const byte pinLed = LED_BUILTIN;

void ledOn(bool on, ulong itv = 0)
{
    digitalWrite(pinLed, !on);
    if (itv > 0)
        delay(itv);
}

void blink(int itv = 500)
{
    ledOn(true, itv);
    ledOn(false, itv);
}

void blinks(uint n, int itv = 500)
{
    for (uint i = 0; i < n; i++)
        blink(itv);
}

// Konektovanje na WiFi, postavljanje IP adrese i startovanje veb servera.
void WiFiOn()
{
    ledOn(true);
    WiFi.mode(WIFI_STA);
    if (!ConnectToWiFi())
    {
        // for (ulong i = 0; i < 3; i++)
        //     blink(500);
        blinks(3);
        Serial.println("Sleep...");
        ESP.deepSleep(5 * 1000000, RF_DISABLED);
    }
    blinks(5);
    Serial.println("Setup address...");
    SetupIPAddress(55);
    server.on("/", []()
              { server.send(200, "text/plain", "ESP Web server is working."); });
    server.begin();
    ledOn(false);
}

// Diskonektovanje sa WiFi-a.
void WiFiOff()
{
    server.stop();
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    WiFi.forceSleepBegin();
    delay(100);
}

void setup()
{
    Serial.begin(115200);
    Serial.println("\n ------- setup start ------");
    pinMode(pinLed, OUTPUT);
    WiFiOn();
}

void loop()
{
    delay(10);
    server.handleClient();
}
