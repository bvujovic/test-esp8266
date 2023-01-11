/*
  ESP32 WiFi Signal Test
  esp32-wifi-signal-test.ino
  Tests WiFi signal strength
  *Requires WiFi SSID and Password

  DroneBot Workshop 2020
  https://dronebotworkshop.com
*/

#include "ESP8266WiFi.h"
// #include <CredWiFi.h>
const char *WIFI_SSID = "Vujovic";
const char *WIFI_PASS = "ritern123";

void setup()
{
    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print("-");
    }
    Serial.println("");
    Serial.print("Connected to network ");
    Serial.println(WIFI_SSID);
    delay(100);
}

void loop()
{
    Serial.print(WIFI_SSID);
    Serial.print(" Signal Level: ");
    Serial.println(WiFi.RSSI());
    delay(5000);
}