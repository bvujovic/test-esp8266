#include "Arduino.h"
#include "WiFiServerBasics.h"
#include <ESP8266httpUpdate.h>
#include <EasyFS.h>

const int pinLed = LED_BUILTIN;
const int maxUpdate = 20;
int cntUpdate = 0;

void ledOn(bool on) { digitalWrite(pinLed, !on); }

void setup()
{
  pinMode(pinLed, OUTPUT);
  Serial.begin(115200);
}

// void update_started() { Serial.println("CALLBACK:  HTTP update process started"); }
// void update_finished() { Serial.println("CALLBACK:  HTTP update process finished"); }
// void update_progress(int cur, int total) { Serial.printf("CALLBACK:  HTTP update process at %d of %d bytes (%d%%) ...\n", cur, total, 100 * cur / total); }
// void update_error(int err) { Serial.printf("CALLBACK:  HTTP update fatal error code %d\n", err); }

void loop()
{
  const int itv = 500;
  ledOn(true);
  delay(itv);
  ledOn(false);
  delay(itv);

  if (++cntUpdate > maxUpdate)
  {
    cntUpdate = 0;
    ledOn(true);
    WiFi.mode(WIFI_STA);
    Serial.println(1);
    // if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println(2);
      ConnectToWiFi();
      SetupIPAddress(44);
      Serial.println(WiFi.localIP());
      Serial.println(3);
    }

    WiFiClient client;
    ESPhttpUpdate.setLedPin(pinLed, LOW);
    // ESPhttpUpdate.onStart(update_started);
    // ESPhttpUpdate.onEnd(update_finished);
    // ESPhttpUpdate.onProgress(update_progress);
    // ESPhttpUpdate.onError(update_error);
    HTTPUpdateResult res;

    Serial.println("Start...");
    res = ESPhttpUpdate.updateFS(client, "http://kingtrader.info/esp/WeMos_ESP8266/littlefs.bin");
    if (res != HTTP_UPDATE_OK)
      Serial.printf("Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
    else
      Serial.println(EasyFS::read("/test.txt"));

    // res = ESPhttpUpdate.update(client, "http://kingtrader.info/esp/WeMos_ESP8266/firmware.bin");
    // if (res != HTTP_UPDATE_OK)
    //   Serial.printf("Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
  }
}