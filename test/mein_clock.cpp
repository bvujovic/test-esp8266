//* Device beeps every hour, half an hour, 10min depending on positions of 3 switches.
//* Future improvements: display, countdown timer, menu, sleep mode
//* Full project: https://github.com/bvujovic/MeinClock

#include <Arduino.h>
#include "ESP8266WiFi.h"
#include "CredWiFi.h"

#include <Blinky.h> // https://github.com/bvujovic/ArduinoLibs/tree/main/Blinky
Blinky led = Blinky::create();
Blinky buzzer = Blinky(D8, true);

const byte pinHour = D5;
const byte pin30min = D6;
const byte pin10min = D7;

#include <SNTPtime.h>
SNTPtime ntp;
StrDateTime now;

bool isTimeSet;
ulong cntTrySetTime = 0;
const ulong maxTrySetTime = 3;

// Signal that time is not set so device can't run properly.
void noTimeError()
{
  buzzer.blinkErrorMajor();
  led.on();
}

void getCurrentTime()
{
  isTimeSet = false;
  cntTrySetTime = 0;
  while (!ntp.setSNTPtime() && cntTrySetTime++ < maxTrySetTime)
    Serial.print("*");
  Serial.println();
  if (cntTrySetTime < maxTrySetTime)
  {
    isTimeSet = true;
    Serial.println("Time set");
  }
  else
  {
    Serial.println("Time NOT set");
    noTimeError();
  }
}

void wiFiOff()
{
  Serial.println("Turning WiFi OFF...");
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  delay(100);
  Serial.println("WiFi OFF");
  led.off();
}

// Is pin set so that the buzzer should go on for (1h - pinHour or 30min - pin30min...)
bool isPinSet(byte pin)
{
  if (pin != pinHour && pin != pin30min && pin != pin10min)
    return false;
  else
    return !digitalRead(pin);
}

void setup()
{
  pinMode(pinHour, INPUT_PULLUP);
  pinMode(pin30min, INPUT_PULLUP);
  pinMode(pin10min, INPUT_PULLUP);
  pinMode(buzzer.getPin(), OUTPUT);
  pinMode(led.getPin(), OUTPUT);
  buzzer.off();
  led.on();
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to network: ");
  Serial.println(WIFI_SSID);
  delay(100);

  getCurrentTime();
  wiFiOff();
}

void loop()
{
  delay(1000);
  now = ntp.getTime(1.0, 1);

  // T
  // now.Println();
  // led.blink(1);

  if (now.second == 0)
  {
    if (now.minute == 0) // 1h
    {
      if (isPinSet(pinHour))
        buzzer.blink(1000, 2);
    }
    else if (now.minute == 30) // 30min
    {
      if (isPinSet(pin30min))
        // buzzer.blink(500, 2);
        buzzer.blink(1000, 1);
    }
    else if (now.minute % 10 == 0) // 10min
    {
      if (isPinSet(pin10min))
        // buzzer.blink(500, 1);
        buzzer.blink(333, (now.minute == 10 || now.minute == 40) ? 1 : 2); // 10, 40 - 1x, 20, 50 - 2x
    }
  }
}
