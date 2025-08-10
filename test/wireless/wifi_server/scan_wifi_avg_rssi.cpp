#include "ESP8266WiFi.h"

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

void loop()
{
    Serial.print("Scan start... ");

    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    Serial.println("Done.");
    Serial.println(String(n) + " networks found");
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
    delay(10000);
}
