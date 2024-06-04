//*...

#include <Arduino.h>
#include "ESP8266WiFi.h"
#include "CredWiFi.h"
#include <SNTPtime.h>

SNTPtime ntp;
StrDateTime now;

#define SEC (1000)

void wiFiOff()
{
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    WiFi.forceSleepBegin();
}

bool getCurrentTime()
{
    const ulong maxTrySetTime = 3;
    ulong cntTrySetTime = 0;
    while (!ntp.setSNTPtime() && cntTrySetTime++ < maxTrySetTime)
        ;
    return cntTrySetTime < maxTrySetTime;
}

bool initTime()
{
    // WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED)
        delay(250);
    delay(100);
    if (WiFi.status() == WL_CONNECTED)
    {
        bool res = getCurrentTime();
        wiFiOff();
        return res;
    }
    return false;
}

uint32_t bootcount;
const uint32_t bcMax = 10 * 60 / 2; // x min * 60 sec / 2 (tick/sec)
const uint32_t itvWakeUp = 210; // 

void setup()
{
    Serial.begin(115200);
    if (!ESP.rtcUserMemoryRead(0, &bootcount, sizeof(bootcount)))
    {
        Serial.println("RTC read failed!");
        ESP.deepSleep(0);
    }
    if (bootcount > bcMax)
    {
        initTime();
        wiFiOff();
        now = ntp.getTime(1.0, 1);
        now.Println();
        bootcount = 0;
    }
    bootcount++;
    if (!ESP.rtcUserMemoryWrite(0, &bootcount, sizeof(bootcount)))
    {
        Serial.println("RTC write failed!");
        ESP.deepSleep(0);
    }
    Serial.print("boot count is ");
    Serial.println(bootcount);

    if (bootcount >= 1 * bcMax)
    {
        ulong ms = millis();
        initTime();
        wiFiOff();
        now = ntp.getTime(1.0, 1);
        now.Println();
        Serial.println(millis() - ms);
        ESP.deepSleep(0);
    }
    ESP.deepSleep((2 * SEC - itvWakeUp) * 1000);
}

void loop() {}
