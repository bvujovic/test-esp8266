/*
  ESP32 WiFi Signal Test
  esp32-wifi-signal-test.ino
  Tests WiFi signal strength
  *Requires WiFi SSID and Password

  DroneBot Workshop 2020
  https://dronebotworkshop.com
*/

#include "ESP8266WiFi.h"
#include <CredWiFi.h>

void MySetupIPAddress(IPAddress ipa)
{
    if (WiFi.isConnected())
    {
        // IPAddress ipa(192, 168, 0, ipLastNum);
        IPAddress subnet(255, 255, 255, 0);
        IPAddress gateway(192, 168, 0, 254);
        WiFi.config(ipa, gateway, subnet);
        Serial.print("IP address set: ");
        Serial.println(WiFi.localIP());
    }
    else
        Serial.println("SetupIPAddress: Error!");
}

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

    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    //! pokrenuti ipconfig i proveriti gateway i subnet vrednosti
    IPAddress ipa(10, 106, 164, 123);
    // IPAddress ipa(192, 168, 0, 123);
    MySetupIPAddress(ipa);

    delay(100);
}

void loop()
{
    Serial.print(WIFI_SSID);
    Serial.print(" Signal Level: ");
    Serial.println(WiFi.RSSI());
    delay(5000);
}