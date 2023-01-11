#include <SoftwareSerial.h>
SoftwareSerial ss(D3, D2);

#include <TinyGPS++.h>
TinyGPSPlus gps;

// #include <Primer.h>

// #include "TM1637Display.h"
// // TM1637Display *display;

// #include <Tst.h>

void setup()
{
    Serial.begin(115200);
    ss.begin(9600);
}

void loop()
{
    // while (ss.available() > 0)
    // {
    //     byte gpsData = ss.read();
    //     Serial.write(gpsData);
    // }

    while (ss.available() > 0)
    {
        gps.encode(ss.read());
        if (gps.location.isUpdated())
        {
gps.date.year();

            Serial.print("Latitude= ");
            Serial.print(gps.location.lat(), 6);
            Serial.print(" Longitude= ");
            Serial.println(gps.location.lng(), 6);
        }
        // else
        // {
        //     Serial.println("*");
        // }
        
    }

    Serial.println();
    delay(5000);
}