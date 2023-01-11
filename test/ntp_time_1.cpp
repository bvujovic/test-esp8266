//* Na bazi: https://werner.rothschopf.net/202011_arduino_esp8266_ntp_en.htm
//* Aparat pisti jednom na pola sata i dvaput kada je pun sat. Vreme se azurira
//* jednom dnevno kada je WiFi ukljucen, inace je iskljucen.
//* Da li bi moglo da se ubaci neki delta/offset koji bi resio "brz"/"spor" sat
//* izmedju 2 uzimanja tekuceg vremena sa interneta

#include <Arduino.h>
#include "CredWiFi.h"

/* Configuration of NTP */
#define MY_NTP_SERVER "rs.pool.ntp.org"
#define MY_TZ "CET-1CEST,M3.5.0/02,M10.5.0/03"

/* Necessary Includes */
#include <ESP8266WiFi.h> // we need wifi to get internet access
#include <time.h>        // time() ctime()

/* Globals */
time_t now;   // this is the epoch
struct tm tm; // the structure tm holds time information in a more convient way

#include <coredecls.h> // optional settimeofday_cb() callback to check on server
void time_is_set()
{
    Serial.println(F("time was set!!"));
}
// uint32_t sntp_update_delay_MS_rfc_not_less_than_15000()
// {
//     //* Ovo podesavanje nije bitno jer se vreme azurira jednom dnevno (21:52) kad se WiFi ukljuci
//     // return 12 * 60 * 60 * 1000UL; // 12 hours
//     // return 30 * 1000UL; // 30 secs
//     // return 1 * 60 * 60 * 1000UL; // 1 hours
//     return 5 * 60 * 1000UL; // 5 min
// }

const byte pinBuzz = 10;

void signal(byte pin, bool on, int itv = 0)
{
    digitalWrite(pin, on);
    if (itv > 0)
    {
        delay(itv);
        digitalWrite(pin, !on);
    }
}

void buzz(int itv)
{
    signal(pinBuzz, true, itv);
}

// Ukljucivanje/iskljucivanje ugradjene LED diode
// Ako je itv > 0, LED je uklj/isklj itv milisekundi
void ledOn(bool on, int itv = 0)
{
    signal(LED_BUILTIN, !on, itv);
    // digitalWrite(LED_BUILTIN, !on);
    // if (itv > 0)
    // {
    //     delay(itv);
    //     digitalWrite(LED_BUILTIN, on);
    // }
}

void getTime()
{
    time(&now);             // read the current time
    localtime_r(&now, &tm); // update the structure tm with the current time
}

void showTime()
{
    Serial.print("year:");
    Serial.print(tm.tm_year + 1900); // years since 1900
    Serial.print("\tmonth:");
    Serial.print(tm.tm_mon + 1); // January = 0 (!)
    Serial.print("\tday:");
    Serial.print(tm.tm_mday); // day of month
    Serial.print("\thour:");
    Serial.print(tm.tm_hour); // hours since midnight  0-23
    Serial.print("\tmin:");
    Serial.print(tm.tm_min); // minutes after the hour  0-59
    Serial.print("\tsec:");
    Serial.print(tm.tm_sec); // seconds after the minute  0-61*
    Serial.print("\twday");
    Serial.print(tm.tm_wday); // days since Sunday 0-6
    if (tm.tm_isdst == 1)     // Daylight Saving Time flag
        Serial.print("\tDST");
    else
        Serial.print("\tstandard");
    Serial.println();
}

// Diskonektovanje sa WiFi-a.
void wiFiOff()
{
    if (WiFi.getMode() == WIFI_OFF)
        return;
    Serial.println("Turning WiFi OFF...");
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    WiFi.forceSleepBegin();
    delay(100);
    Serial.println("WiFi OFF");
    ledOn(false);
}

void wiFiOn()
{
    ledOn(true); // LED se gasi na WiFiOff
    // WiFi.persistent(false);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(200);
        Serial.print(".");
    }
}

bool isItTime(int hour, int min, int sec)
{
    return tm.tm_hour == hour && tm.tm_min == min && tm.tm_sec == sec;
}

void setup()
{
    pinMode(pinBuzz, OUTPUT);
    digitalWrite(pinBuzz, false);
    pinMode(LED_BUILTIN, OUTPUT);
    ledOn(false);

    Serial.begin(115200);

    // pinMode(10, OUTPUT);
    // digitalWrite(10, true);
    // delay(500);
    // digitalWrite(10, false);
    // while (true)
    //     delay(100);

    settimeofday_cb(time_is_set);     // optional: callback if time was set
    configTime(MY_TZ, MY_NTP_SERVER); // --> Here is the IMPORTANT ONE LINER needed in your sketch!

    wiFiOn();
}

// B int i = 0;

void loop()
{
    // B
    //  i++;
    //  Serial.print(i);
    //  Serial.print('\t');
    //  Serial.print(tm.tm_year);
    //  Serial.print('\t');
    //  Serial.print(tm.tm_mon);
    //  Serial.print('\n');

    delay(1000);

    getTime();
    // B if (tm.tm_year <= 0)
    if (tm.tm_year < 122) // ako nije 1900 + 122 = 2022 godina ili neka veca -> vreme nije dobro podeseno
        return;

    // B if (i == 30)
    if (WiFi.getMode() != WIFI_OFF)
        wiFiOff();

    // unsigned long ms = millis();
    // Serial.println(millis() - ms);
    // showTime();

    // konektovanje na wifi
    // if (tm.tm_yday % 2 == 0 && tm.tm_hour == 21 && tm.tm_sec == 0)
    if (tm.tm_sec == 0)
    {
        if (tm.tm_min % 10 == 0)
            wiFiOn();
        if (tm.tm_min % 10 == 1)
            wiFiOff();
    }

    // // paljenje AQ svetla
    // if (isItTime(15, 30, 0))
    //     ledOn(true, 1000);
    // // gasenje AQ svetla
    // if (isItTime(19, 30, 0))
    //     ledOn(true, 1000);

    // if (tm.tm_min == 59 && tm.tm_sec == 50)
    //     ledOn(true, 1000);
    // if (tm.tm_min == 0 && tm.tm_sec == 0)
    //     ledOn(true, 2000);

    if (tm.tm_sec == 0)
    {
        if (tm.tm_min == 30)
            buzz(50);
        if (tm.tm_min == 0)
        {
            buzz(50);
            delay(100);
            buzz(50);
        }
    }
}
