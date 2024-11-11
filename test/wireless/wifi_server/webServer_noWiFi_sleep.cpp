//* Programski kôd za eksperiment: koliko naizmenične struje troši montaža 220V->5V i ESP8266 u
//* 3 slučaja-faze: veb server, diskonektovan sa mreže, deep sleep. Ovo su rezultati:
//*             ~I(mA)  ~P(mW)  W za dan    W za mesec
//* Server      2.5     550     13.2        403
//* No WiFi     1       220     5.3         162
//* Sleep       0.5     110     2.6         80
//* Realni aparati će trošiti više struje zbog dodatnih komponenti.

#include <Arduino.h>
typedef unsigned long ulong;

#include <WiFiServerBasics.h>

#if defined(ESP8266)
ESP8266WebServer server(80);
const byte pinLed = LED_BUILTIN;
#elif defined(ESP32)
WebServer server(80);
const byte pinLed = 2;
#endif

void ledOn(bool on, ulong itv = 0)
{
    digitalWrite(pinLed, !on);
    if (itv > 0)
        delay(itv);
}

void bigBlink()
{
    ledOn(false, 500);
    ledOn(true, 2000);
    ledOn(false);
}

// Konektovanje na WiFi, postavljanje IP adrese i startovanje veb servera.
void WiFiOn()
{
    ledOn(true);
    WiFi.mode(WIFI_STA);
    if (!ConnectToWiFi())
        for (ulong i = 0; i < 6; i++)
            ledOn(i % 2, 500);
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
#if defined(ESP8266)
    WiFi.forceSleepBegin();
#endif
    delay(100);
}

enum Faza
{
    WebServerPhase,
    NoWiFi,
    Sleep
} faza;

const ulong itvTrajanjeFaze = 60000 * 10;
ulong msPromenaFaze;

void setup()
{
    pinMode(pinLed, OUTPUT);
    WiFiOn();
    faza = WebServerPhase;
    msPromenaFaze = millis();
}

void loop()
{
    delay(10);

    if (faza == WebServerPhase)
    {
        server.handleClient();
        if (millis() > msPromenaFaze + itvTrajanjeFaze)
        {
            bigBlink();
            faza = NoWiFi;
            WiFiOff();
            msPromenaFaze = millis();
        }
    }

    if (faza == NoWiFi)
    {
        if (millis() > msPromenaFaze + itvTrajanjeFaze)
        {
            bigBlink();
            ESP.deepSleep(itvTrajanjeFaze * 1000);
        }
    }
}