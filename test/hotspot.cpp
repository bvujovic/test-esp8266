#include <Arduino.h>
//#include <WiFi.h>
#include "ESP8266WiFi.h"

const char *wifi_network_ssid = "Vujovic";
const char *wifi_network_password = "ritern123";

const char *soft_ap_ssid = "BojanESP";
const char *soft_ap_password = "testpassword";

void setup()
{

    Serial.begin(115200);

    WiFi.mode(WIFI_AP_STA);

    WiFi.softAP(soft_ap_ssid, soft_ap_password);
    WiFi.begin(wifi_network_ssid, wifi_network_password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("Connecting to WiFi..");
    }

    Serial.print("ESP32 IP as soft AP: ");
    Serial.println(WiFi.softAPIP());

    Serial.print("ESP32 IP on the WiFi network: ");
    Serial.println(WiFi.localIP());
}

void loop() { }
