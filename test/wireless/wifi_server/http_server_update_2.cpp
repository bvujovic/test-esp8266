#include "Arduino.h"
#include "WiFiServerBasics.h"
#include <ESP8266httpUpdate.h>
#include <EasyFS.h>

#include <EEPROM.h>
const int eepromPos = 66;

const int pinLed = LED_BUILTIN;
ESP8266WebServer server(80);

void ledOn(bool on) { digitalWrite(pinLed, !on); }

void handleOTA()
{
  server.send(200, "text/plain", "ESP is waiting for OTA updates...");
  delay(100);
  EEPROM.write(eepromPos, true);
  EEPROM.commit();
  ESP.reset();
}

void setup()
{
  pinMode(pinLed, OUTPUT);
  ledOn(false);
  Serial.begin(115200);
  // Serial.println("*******");
  LittleFS.begin();
  EEPROM.begin(100);

  byte isOta = EEPROM.read(eepromPos);

  WiFi.mode(WIFI_STA);
  if (WiFi.status() != WL_CONNECTED)
    ConnectToWiFi();

  if (isOta)
  {
    EEPROM.write(eepromPos, false);
    EEPROM.commit();
    Serial.println("OTA...");
    WiFiClient client;
    ESPhttpUpdate.setLedPin(pinLed, LOW);
    HTTPUpdateResult res = ESPhttpUpdate.updateFS(client, "http://kingtrader.info/esp/WeMos_ESP8266/littlefs.bin");
    if (res != HTTP_UPDATE_OK)
      Serial.println("HTTP Server Update FS Error: " + ESPhttpUpdate.getLastErrorString());
    else
    {
      res = ESPhttpUpdate.update(client, "http://kingtrader.info/esp/WeMos_ESP8266/firmware.bin");
      if (res != HTTP_UPDATE_OK)
        Serial.println("HTTP Server Update sketch Error: " + ESPhttpUpdate.getLastErrorString());
    }
  }
  SetupIPAddress(44);

  server.on("/test1", []() { server.send(200, "text/html", "<h1>Test1: You are connected</h1>"); });
  server.on("/test2", []() { server.send(200, "text/html", "<h1>Test2: " + EasyFS::read("/test.txt") + "</h1>"); });
  server.on("/ota", handleOTA);
  server.begin();
}

void loop()
{
  delay(10);
  server.handleClient();
}