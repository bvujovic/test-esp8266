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
    url = url + "&text=" + "Beavis!+++test:)";
    // U poruci ne smeju da budu spec. karakteri '\n' i ' '. Za spejs moze da se koristi '+'.
    url = url + "&apikey=" + CMB_API_KEY;
    Serial.println(url);
// http://api.callmebot.com/whatsapp.php?source=web&phone=+381665501888&apikey=9718584&text=Beavis!\nThe\r\ntest%20:)%F0%9F%99%82

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