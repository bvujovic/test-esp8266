//* Simuliranje 2 neuspesna pokusaja konektovanja na WiFi (sa pogresnom sifrom) i zatim
//* uspesno konektovanje sa pravom.

#include <Arduino.h>
typedef unsigned long ulong;

#include <ESP8266WiFi.h>
#include <CredWiFi.h>

const byte pinLed = LED_BUILTIN;

void ledOn(bool on, ulong itv = 0)
{
    digitalWrite(pinLed, !on);
    if (itv > 0)
        delay(itv);
}

bool ConnectToWiFi(const char *pass)
{
    Serial.print("Connecting to ");
    Serial.print(WIFI_SSID);
    WiFi.begin(WIFI_SSID, pass);
    int i = 0;
    while (WiFi.status() != WL_CONNECTED && i++ < 10)
    {
        delay(1000);
        Serial.print(".");
    }
    Serial.println();
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    return WiFi.status() == WL_CONNECTED;
}

// Konektovanje na WiFi, postavljanje IP adrese i startovanje veb servera.
void WiFiOn(const char *pass)
{
    Serial.print("pass: ");
    Serial.println(pass);
    ledOn(true);
    WiFi.mode(WIFI_STA);
    if (!ConnectToWiFi(pass))
        for (ulong i = 0; i < 6; i++)
            ledOn(i % 2, 500);
    ledOn(false);
}

// Diskonektovanje sa WiFi-a.
void WiFiOff()
{
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    WiFi.forceSleepBegin();
    delay(100);
}

int iConnect = 0; // brojac konektovanja na wifi

void setup()
{
    Serial.begin(115200);
    pinMode(pinLed, OUTPUT);
}

void loop()
{
    iConnect++;
    Serial.println(iConnect);
    if (iConnect < 3)
        WiFiOn("123456");
    else
        WiFiOn(WIFI_PASS);

    if (WiFi.status() == WL_CONNECTED)
        for (size_t i = 0;; i++)
            ledOn(i % 2 == 0, 1000);

    // ESP.deepSleep(5 * 1000000);
}