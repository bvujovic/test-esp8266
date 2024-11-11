//* Device will get current time and count seconds in wake-sleep cycles (1 cycle = 2sec).
//* Goal is to measure time using the least amount of power while giving user the opportunity
//* to see current status and stop the counting (pressing the button while ESP is waking up).

#include <Arduino.h>
#include "ESP8266WiFi.h"
#include "CredWiFi.h"
#include <SNTPtime.h>

SNTPtime ntp;
StrDateTime now;
uint32_t counter;

#define SEC (1000)    // msec in 1 sec
#define CYCLE_SEC (2) // 1 wake-sleep cycle - how many seconds

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

void wifiOnTimeOff()
{
    initTime();
    wiFiOff();
    now = ntp.getTime(1.0, 1);
    now.Println();
}

void saveCountToRTC()
{
    if (!ESP.rtcUserMemoryWrite(0, &counter, sizeof(counter)))
    {
        Serial.println("RTC write failed!");
        ESP.deepSleep(0);
    }
}

const uint32_t bcMax = 10 * 60 / CYCLE_SEC; // x min * 60 sec / 2 (tick/sec)
const uint32_t itvWakeUpESP = 220;          // how many msec ESP needs to wake up w/o setup()
uint32_t itvSetup;                          // time (msec) to execute setup()
ulong ms;
const byte pinBtn = D1;

void setup()
{
    ms = millis();
    pinMode(pinBtn, INPUT_PULLUP);
    if (!ESP.rtcUserMemoryRead(0, &counter, sizeof(counter)))
    {
        Serial.println("RTC read failed!");
        ESP.deepSleep(0);
    }

    Serial.begin(115200);
    if (digitalRead(pinBtn)) // button NOT pressed
    {
        if (counter == ULONG_MAX) // start counting
        {
            counter = 0;
            wifiOnTimeOff();
        }
        else
            counter += CYCLE_SEC;
        Serial.println(counter);
    }
    else // button PRESSED
    {
        if (counter > 0) // stop counting
        {
            Serial.printf("\nCount: %d\n", counter + CYCLE_SEC);
            ulong msEnd = millis();
            wifiOnTimeOff();
            Serial.println(millis() - msEnd);
            counter = ULONG_MAX;
            saveCountToRTC();
            ESP.deepSleep(0);
        }
    }
    saveCountToRTC();
    itvSetup = counter > 0 ? millis() - ms : 0;
    ESP.deepSleep((CYCLE_SEC * SEC - itvWakeUpESP - itvSetup) * 1000);
}

void loop() {}
