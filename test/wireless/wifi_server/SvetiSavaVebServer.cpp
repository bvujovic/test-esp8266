//* Testiranje da li je moguć rad veb servera na školskoj AMRES mreži (Sveti Sava)
//* Ova verzija koda NE funkcioniše.

#include <Arduino.h>
typedef unsigned long ulong;

#include <WiFiServerBasics.h>
#include <CredWiFi_SvetiSava.h> // Credentials for WiFi connection

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

void blink(int n, ulong itv = 500)
{
    for (int i = 0; i < n; i++)
    {
        ledOn(true, itv);
        ledOn(false, itv);
    }
}

void MySetupIPAddress(IPAddress ipa)
{
    if (WiFi.isConnected())
    {
        // IPAddress ipa(192, 168, 0, ipLastNum);
        IPAddress subnet(255, 255, 255, 0);
        IPAddress gateway(192, 168, 0, 254);
        auto res = WiFi.config(ipa, gateway, subnet);
        Serial.println("WiFi.config result: " + String(res));
        ledOn(false, 100);
        if (res)
            blink(2);
        else
            blink(5, 100);
        Serial.print("IP address set: ");
        Serial.println(WiFi.localIP());
    }
    else
        Serial.println("SetupIPAddress: Error!");
}

void setup()
{
    Serial.begin(115200);
    Serial.println("\nStarting ESP Web Server...");
    pinMode(pinLed, OUTPUT);
    ledOn(true);
    WiFi.mode(WIFI_STA);
    if (!ConnectToWiFi(WIFI_SSID, WIFI_PASS))
        for (ulong i = 0; i < 6; i++)
            ledOn(i % 2, 500);
    // SetupIPAddress(123);
    IPAddress ipa(10, 106, 164, 123);
    MySetupIPAddress(ipa);
    server.on("/", []()
              { server.send(200, "text/plain", "ESP Web server is working."); });
    server.begin();
    ledOn(false);
}

void loop()
{
    delay(10);
    server.handleClient();
}
