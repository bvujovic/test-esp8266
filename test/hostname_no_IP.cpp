//* https://www.youtube.com/watch?v=1CXl2qCG3Qk
//* radi na desktopu, ne radi na mobilnom 

#include <Arduino.h>
#include <WiFiServerBasics.h>
ESP8266WebServer server(80);
#include <ESP8266mDNS.h>

void setup()
{
    Serial.begin(115200);
    Serial.println();
    WiFi.mode(WIFI_STA);
    WiFi.setAutoConnect(false);
    WiFi.setAutoReconnect(false);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    int i = 0;
    while (WiFi.status() != WL_CONNECTED && i++ < 10)
    {
        delay(1000);
        Serial.print(".");
    }
    Serial.println();
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println(WiFi.hostname());

    server.on("/", []()
              { server.send(200, "text/plain", "ESP Web server is working."); });
    server.begin();

    MDNS.begin("test123");
}

void loop()
{
    server.handleClient();
    MDNS.update();
    // delay(10);
}
