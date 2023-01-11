//*

#include <WiFiServerBasics.h>
ESP8266WebServer server(80);
bool isServerSet = false;

#include <SNTPtime.h> // https://github.com/SensorsIot/SNTPtime/
SNTPtime *ntp = NULL;
StrDateTime now;
bool isTimeSet;
uint32 cntTrySetTime = 0;
const uint32 maxTrySetTime = 3;

#include <EasyFS.h>

#include <ClickButton.h>
// taster: startovanje WiFi-a (dugi klik), OTA update-a (dugi klik dok je WiFi ON), moment-on svetlo (kratki klikovi)
ClickButton btn(D3, LOW, CLICKBTN_PULLUP);

void GetCurrentTime();

void WiFiOn()
{
    Serial.println("conn...");
    WiFi.mode(WIFI_STA);
    ConnectToWiFi();
    Serial.println("connected");

    GetCurrentTime();
    SetupIPAddress(123);
    if (isServerSet)
    {
        server.on("/", []()
                  { Serial.println("bleja..."); });
        server.on("/otaUpdate", []()
                  { server.send(200, "text/plain", "ESP is waiting for OTA updates..."); });
        isServerSet = true;
    }
    server.begin();
}

// Diskonektovanje sa WiFi-a.
void WiFiOff()
{
    Serial.println("Turning WiFi OFF...");
    server.stop();
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    WiFi.forceSleepBegin();
    delay(100);
    Serial.println("WiFi OFF");
}

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, true);
    Serial.begin(115200);
    WiFiOn();
    EasyFS::setFileName("/msg.log");
}

void LogMessage(const String &msg)
{
    char strDateTime[32];
    sprintf(strDateTime, "%04d-%02d-%02d %02d:%02d:%02d", now.year, now.month, now.day, now.hour, now.minute, now.second);
    EasyFS::addf(msg);
    EasyFS::addf(strDateTime);
}

void GetCurrentTime()
{
    uint32 start = millis();
    if (ntp != NULL)
        delete ntp;
    ntp = new SNTPtime();
    isTimeSet = false;
    cntTrySetTime = 0;
    while (!ntp->setSNTPtime() && cntTrySetTime++ < maxTrySetTime)
        Serial.print("*");
    Serial.println();
    if (cntTrySetTime < maxTrySetTime)
    {
        isTimeSet = true;
        Serial.println("Time set");
    }
    uint32 msec = millis() - start;
    now = ntp->getTime(1.0, 1);
    // Serial.println(String("GetCurrentTime: ") + msec + " ms, free: " + ESP.getFreeHeap() + " bytes");
    LogMessage(String("GetCurrentTime: ") + msec + " ms, free: " + ESP.getFreeHeap() + " bytes");
}

uint32 msLastPrint = 0;
int counter = 10;

void loop()
{
    delay(10);

    btn.Update();
    if (btn.clicks >= 1)
        ;

    if (counter > 0 && millis() > msLastPrint + 5000)
    {
        // Serial.println(String("GetCurrentTime: ") + msLastPrint + " ms, free: " + ESP.getFreeHeap() + " bytes");
        // msLastPrint = millis();
        WiFiOn();
        now = ntp->getTime(1.0, 1);
        Serial.println(ESP.getFreeHeap());
        now.Println();
        WiFiOff();
        msLastPrint = millis();
        counter--;
    }
}
