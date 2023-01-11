#include <SPI.h>
#include <LoRa.h>

int counter = 0;

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(9600);
    while (!Serial)
        ;
    Serial.println("LoRa Sender");

    // for (size_t i = 0; i <= 20; i++)
    // {
    //     Serial.println(i);
    //     digitalWrite(LED_BUILTIN, i % 2);
    //     delay(333);
    // }

    // while (true)
    //     delay(100);

    if (!LoRa.begin(433E6))
    {
        Serial.println("Starting LoRa failed!");
        while (1)
            ;
    }
}

void loop()
{
    Serial.print("Sending packet: ");
    Serial.println(counter);

    LoRa.beginPacket();
    // LoRa.print("hello ");
    LoRa.print(counter);
    LoRa.endPacket();

    counter++;

    delay(1000);
}