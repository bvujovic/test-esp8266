#include <SPI.h>
#include <LoRa.h>

const byte pinLed = A4;

void setup()
{
    pinMode(pinLed, OUTPUT);

    Serial.begin(9600);
    while (!Serial)
        ;

    Serial.println("LoRa Receiver");

    // if (!LoRa.begin(915E6))
    if (!LoRa.begin(433E6))
    {
        Serial.println("Starting LoRa failed!");
        while (1)
            ;
    }
}

void loop()
{
    // try to parse packet
    int packetSize = LoRa.parsePacket();
    if (packetSize)
    {
        // received a packet
        // Serial.print("Received packet '");

        // read packet
        char ch = 0;
        while (LoRa.available())
        {
            // B Serial.print((char)LoRa.read());
            ch = LoRa.read();
            Serial.print(ch);
        }
        // Serial.println();
        // Serial.println((int)ch);
        // Serial.println(ch % 2);
        digitalWrite(pinLed, ch % 2);
        // if (ch % 2 == 0)
        // {
        //     Serial.println("true");
        //     digitalWrite(pinLed, true);
        // }
        // else
        // {
        //     Serial.println("false");
        //     digitalWrite(pinLed, false);
        // }

        // print RSSI of packet
        // Serial.print("' with RSSI ");
        // Serial.println(LoRa.packetRssi());
    }
}