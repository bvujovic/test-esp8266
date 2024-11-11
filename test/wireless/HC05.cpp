//* Test za HC-05 bluetooth modul.
//* Modul ne uspevam da prebacim u cmd mode.
//* https://www.youtube.com/watch?v=bP_lscs3MoQ
//* https://www.youtube.com/watch?v=hyME1osgr7s

#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial ss(D7, D8); // HC-12 TX Pin, HC-12 RX Pin

void setup()
{
    Serial.begin(115200); // Serial port to computer
    Serial.println("*** start ***");
    ss.begin(9600); // Serial port to HC12
    //   pinMode(8, OUTPUT);
    //   digitalWrite(8, HIGH);
    // delay(1000);
    // ss.println("AT");
}

bool atSent = false;

void loop()
{
    while (ss.available())
    {
        int x = ss.read();
        // Serial.write(x);
        Serial.print((char)x);
        // Serial.println((int)x);
    }
    // while (Serial.available())
    // {                            // If Serial monitor has data
    //     ss.write(Serial.read()); // Send that data to HC-12
    // }
    if (millis() >= 10000 && !atSent)
    {
        atSent = true;
        ss.println("AT");
        Serial.println("AT");
    }
}