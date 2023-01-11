//* Slanje notifikacije sa ESP-a na mobilni telefon (WhatsApp poruka)
//* Ideja sa: https://www.youtube.com/watch?v=ZrkAWSemDC8
//* https://www.callmebot.com/blog/free-api-whatsapp-messages/

#include <Arduino.h>
#include <WiFiServerBasics.h>
#include <ESP8266HTTPClient.h>
#include <CredCallMeBot.h>

const byte pinLed = LED_BUILTIN;

void setup()
{
    pinMode(pinLed, OUTPUT);
    for (size_t i = 0; i < 10; i++)
    {
        digitalWrite(pinLed, i % 2);
        delay(500);
    }

    Serial.begin(115200);
    ConnectToWiFi();

    String url = "http://api.callmebot.com/whatsapp.php?";
    url = url + "phone=" + CMB_PHONE;
    url = url + "&text=" + "Beavis!+The+test.";
    url = url + "&apikey=" + CMB_API_KEY;

    WiFiClient wiFiClient;
    HTTPClient client;
    client.begin(wiFiClient, url);
    int respCode = client.GET();
    Serial.print("Resp code: ");
    Serial.println(respCode);
    if (respCode > 0)
        Serial.println(client.getString());
    client.end();

    Serial.println("Kraj.");
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    WiFi.forceSleepBegin();
}

void loop()
{
    delay(100);
}