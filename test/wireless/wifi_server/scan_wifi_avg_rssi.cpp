// #include "ESP8266WiFi.h"
#include "WiFi.h"

#include "LinkedList.h" // lib_deps = ivanseidel/LinkedList@0.0.0-alpha+sha.dac3874d28
class Net
{
public:
    String SSID;
    int32_t RSSI_sum;
    int32_t RSSI_count;
};
LinkedList<Net *> nets = LinkedList<Net *>();
bool isFirstScan = true;

Net *findNet(String ssid)
{
    for (int i = 0; i < nets.size(); i++)
    {
        Net *net = nets.get(i);
        if (net->SSID == ssid)
            return net;
    }
    return NULL;
}

int compare(Net *&a, Net *&b)
{
    auto counts = b->RSSI_count - a->RSSI_count;
    if (counts != 0)
        return counts;
    auto rssi = b->RSSI_sum - a->RSSI_sum;
    return rssi;
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Setup...");
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(2000);
}

int cntTest = 1;

void loop()
{
    Serial.printf("Scan #%d...", cntTest);
    int n = WiFi.scanNetworks();
    Serial.println("Done.");
    Serial.print(String(n) + " networks found. ");
    for (int i = 0; i < n; ++i)
    {
        if (isFirstScan)
        {
            Net *net = new Net();
            net->SSID = WiFi.SSID(i);
            net->RSSI_sum = WiFi.RSSI(i);
            net->RSSI_count = 1;
            nets.add(net);
        }
        else
        {
            Net *net = findNet(WiFi.SSID(i));
            if (net)
            {
                net->RSSI_sum += WiFi.RSSI(i);
                net->RSSI_count++;
            }
            else
            {
                net = new Net();
                net->SSID = WiFi.SSID(i);
                net->RSSI_sum = WiFi.RSSI(i);
                net->RSSI_count = 1;
                nets.add(net);
            }
        }
        delay(10);
    }
    Serial.println("Scan results:");
    nets.sort(compare);
    Serial.printf("%-20s (AvgRSSI, found times)\n", "SSID");
    n = nets.size() < 8 ? nets.size() : 8; // Limit to top 8 networks
    for (int i = 0; i < n; i++)
    {
        Net *net = nets.get(i);
        // RSSI >-50 is excellent, >-60 is good, >-70 is fair, < -80 is poor
        Serial.printf("%-20s(%.1f, found %2d times)\n", net->SSID.c_str(), (float)net->RSSI_sum / net->RSSI_count, net->RSSI_count);
    }
    Serial.println();
    isFirstScan = false;
    if (++cntTest > 10)
    {
        Serial.println("10 scans done.");
        while (true)
            delay(1000);
    }
    else
        delay(10000);
}

/*
Testing wifi signal strength with different boards and antennas.
Board were placed in front of monitor, about 4m away from router.
Results (average RSSI over 10 scans):

ESP8266 WeMos D1 Mini, no antenna connected
SSID                 (AvgRSSI, found times)
Vujovic             (-63.9, found 10 times)
823144              (-92.0, found 10 times)
SUZANA              (-80.0, found  9 times)
SvetiSavaESP        (-88.0, found  9 times)
Borojevic           (-81.9, found  8 times)
TS-p86C             (-83.6, found  7 times)
CGA2121_YyZubnp_5g  (-86.0, found  6 times)
CGA2121_YyZubnp     (-87.5, found  6 times)

ESP8266 WeMos D1 Mini, WIFI 2.4G 3dbi PCB Antenna IPEX
SSID                 (AvgRSSI, found times)
Vujovic             (-31.3, found 10 times)
Borojevic           (-55.3, found 10 times)
SvetiSavaESP        (-55.6, found 10 times)
CGA2121_YyZubnp     (-65.0, found 10 times)
CGA2121_YyZubnp_5g  (-65.0, found 10 times)
823144              (-72.0, found 10 times)
GOLF 3A             (-80.1, found 10 times)
Sandra              (-85.1, found 10 times)

ESP8266 WeMos D1 Mini, built-in antenna
SSID                 (AvgRSSI, found times)
Vujovic             (-43.2, found 10 times)
SvetiSavaESP        (-61.2, found 10 times)
CGA2121_w2rZjDC     (-67.6, found 10 times)
823144              (-74.4, found 10 times)
GOLF 3A             (-85.0, found 10 times)
SUZANA              (-72.1, found  9 times)
TS-p86C             (-72.2, found  9 times)
Borojevic           (-64.6, found  7 times)

ESP8266 WeMos D1 Mini, 11cm antenna connected to IPEX
SSID                 (AvgRSSI, found times)
Vujovic             (-41.1, found 10 times)
Borojevic           (-56.2, found 10 times)
CGA2121_w2rZjDC     (-58.5, found 10 times)
SUZANA              (-64.7, found 10 times)
TS-p86C             (-65.1, found 10 times)
SvetiSavaESP        (-68.5, found 10 times)
823144              (-71.6, found 10 times)
GOLF 3A             (-80.4, found 10 times)

ESP32 with battery connector, built-in antenna
Vujovic             (-46.4, found 10 times)
Borojevic           (-56.8, found 10 times)
SvetiSavaESP        (-59.2, found 10 times)
TS-p86C             (-63.1, found 10 times)
SUZANA              (-65.6, found 10 times)
CGA2121_w2rZjDC     (-65.9, found 10 times)
823144              (-71.8, found 10 times)
CGA2121_YyZubnp     (-72.5, found 10 times)

ESP-32S, micro USB, narrow, built-in antenna
SSID                 (AvgRSSI, found times)
Vujovic             (-45.8, found 10 times)
SvetiSavaESP        (-61.7, found 10 times)
Borojevic           (-64.5, found 10 times)
CGA2121_w2rZjDC     (-71.3, found 10 times)
TS-p86C             (-80.7, found 10 times)
823144              (-82.9, found 10 times)
Sandra              (-85.4, found 10 times)
SUZANA              (-85.6, found 10 times)

ESP32-C3 Super Mini (blue), larger ceramic antenna
SSID                 (AvgRSSI, found times)
Vujovic             (-41.2, found 10 times)
TS-p86C             (-58.7, found 10 times)
SvetiSavaESP        (-60.5, found 10 times)
SUZANA              (-60.5, found 10 times)
CGA2121_w2rZjDC     (-60.5, found 10 times)
Borojevic           (-63.5, found 10 times)
823144              (-71.6, found 10 times)
CGA2121_YyZubnp     (-74.6, found 10 times)

ESP32-C3 Super Mini (black), smaller red ceramic antenna
SSID                 (AvgRSSI, found times)
Vujovic             (-59.1, found 10 times)
SUZANA              (-73.2, found 10 times)
Borojevic           (-74.6, found 10 times)
CGA2121_w2rZjDC     (-74.9, found 10 times)
TS-p86C             (-76.4, found 10 times)
SvetiSavaESP        (-83.4, found 10 times)
823144              (-88.2, found 10 times)
CGA2121_YyZubnp     (-89.8, found  8 times)

ESP32-C3 Super Mini w/ screen (purple), smaller red ceramic antenna
SSID                 (AvgRSSI, found times)
Vujovic             (-58.5, found 10 times)
SvetiSavaESP        (-66.9, found 10 times)
Borojevic           (-72.9, found 10 times)
CGA2121_w2rZjDC     (-82.1, found 10 times)
CGA2121_YyZubnp_5g  (-83.9, found 10 times)
CGA2121_YyZubnp     (-83.9, found 10 times)
SUZANA              (-84.4, found 10 times)
823144              (-85.0, found 10 times)
*/
