#include <Arduino.h>
#include <DHT.h>      // lib_deps = adafruit/DHT sensor library@^1.4.6
#define DHTPIN D6     // DHT sensor pin on ESP8266 D1 Mini Lite
#define DHTTYPE DHT22 // DHT 22 (AM2302)
DHT dht(DHTPIN, DHTTYPE);


void setup()
{
    Serial.begin(115200);
    Serial.println();
    dht.begin(); // Initialize DHT sensor
    Serial.println("DHT22 sensor initialized.");
}

void loop()
{
    delay(10000);

    // Read temperature as Celsius
    float temperature = dht.readTemperature();
    // Read humidity
    float humidity = dht.readHumidity();

    // Check if any reads failed and exit early (to try again).
    if (isnan(temperature) || isnan(humidity))
    {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    // Print the results to the Serial Monitor
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C, Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
}
