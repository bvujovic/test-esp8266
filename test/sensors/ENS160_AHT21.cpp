//* Based on: https://www.instructables.com/ENS160-AHT21-Sensor-for-Arduino/
// lib_deps =
//     adafruit/Adafruit AHTX0@^2.0.5
//     dfrobot/DFRobot_ENS160@^1.0.1

#include <Arduino.h>
#include <Wire.h>
const byte pinLed = LED_BUILTIN;

#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 aht;

int tempC;
int humidity;

#include <DFRobot_ENS160.h>
#define I2C_COMMUNICATION
DFRobot_ENS160_I2C ens(&Wire, /*I2CAddr*/ 0x53);

void setup()
{
    Serial.begin(115200);
    pinMode(pinLed, OUTPUT);
    digitalWrite(pinLed, HIGH);
    delay(1000);

    while (NO_ERR != ens.begin())
    {
        Serial.println("Communication with ENS160 device failed, please check connection");
        delay(3000);
    }
    Serial.print("ENS160...");
    ens.setPWRMode(ENS160_STANDARD_MODE);
    ens.setTempAndHum(/*temperature=*/26.0, /*humidity=*/30.0);

    if (!aht.begin())
    {
        Serial.println("Could not find AHT? Check wiring");
        while (1)
            delay(10);
    }
    Serial.println("AHT10 or AHT20 found");
}

void loop()
{
    sensors_event_t humidity1, temp; // Tim had to change to  humidity1
    aht.getEvent(&humidity1, &temp); // populate temp and humidity objects with fresh data
    tempC = (temp.temperature);
    humidity = (humidity1.relative_humidity);
    Serial.print("Temp: ");
    Serial.print(tempC);
    Serial.print(", Hum: ");
    Serial.print(humidity);
    Serial.println("% rH");
    delay(5000);

    uint8_t Status = ens.getENS160Status();
    Serial.print("ENS160 status: ");
    Serial.print(Status);
    // 0-Normal operation, 1-Warm-Up (first 3 minutes), 2-Initial Start-Up (first hour after initial power-on)
    uint16_t ECO2 = ens.getECO2();
    Serial.print(",  CO2 equivalent: ");
    Serial.print(ECO2);
    Serial.println(" ppm");
    // Excellent(400 - 600), Good(600 - 800), Moderate(800 - 1000), Poor(1000 - 1500), Unhealthy(> 1500)

    delay(5000);
}
