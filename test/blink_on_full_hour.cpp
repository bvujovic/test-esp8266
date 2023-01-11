#include <Arduino.h>
typedef unsigned long ulong;

#include "ESP8266WiFi.h"
// const char *WIFI_SSID = "Vujovic";
// const char *WIFI_PASS = "ritern123";
const char *WIFI_SSID = "Sveti Sava";
const char *WIFI_PASS = "YDd58yGc";

#include <SNTPtime.h>
SNTPtime ntp;
StrDateTime now;

const byte pinLed = LED_BUILTIN;

void blink(int msDelay = 1000)
{
    digitalWrite(pinLed, false);
    delay(msDelay);
    digitalWrite(pinLed, true);
}

bool isTimeSet;
ulong cntTrySetTime = 0;
const ulong maxTrySetTime = 3;

void getCurrentTime()
{
    isTimeSet = false;
    cntTrySetTime = 0;
    while (!ntp.setSNTPtime() && cntTrySetTime++ < maxTrySetTime)
        Serial.print("*");
    Serial.println();
    if (cntTrySetTime < maxTrySetTime)
    {
        isTimeSet = true;
        Serial.println("Time set");
    }
    else
        Serial.println("Time NOT set");
}

// Diskonektovanje sa WiFi-a.
void wiFiOff()
{
    Serial.println("Turning WiFi OFF...");
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    WiFi.forceSleepBegin();
    delay(100);
    Serial.println("WiFi OFF");
}

void setup()
{
    pinMode(pinLed, OUTPUT);
    digitalWrite(pinLed, true);
    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to network: ");
    Serial.println(WIFI_SSID);
    delay(100);
    getCurrentTime();
    wiFiOff();
}

void loop()
{
    delay(1000);
    now = ntp.getTime(1.0, 1);
    if (now.second == 0)
    {
        now.Println();
        Serial.printf("%02d:%02d\n", now.hour, now.minute);
        blink();
    }
}
