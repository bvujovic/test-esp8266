//* Prints messages received from HC-12 module via SoftwareSerial
//* Blinks built-in LED when receiving

#include <SoftwareSerial.h>

const byte pinRx = D5; // HC-12 RX Pin
const byte pinTx = D7; // HC-12 TX Pin
SoftwareSerial HC12(pinRx, pinTx);

const byte pinLed = LED_BUILTIN;
void ledOn(bool on) { digitalWrite(pinLed, on ? LOW : HIGH); }

void setup()
{
  Serial.begin(115200); // Serial port to computer
  Serial.println("\nHC-12 Receiver Test");
  // HC12.begin(9600);   // Serial port to HC12
  HC12.begin(4800); // Serial port to HC12
  pinMode(pinLed, OUTPUT);
  ledOn(false);
}

String message;

void loop()
{
  if (HC12.available())
  {
    auto now = millis();
    ledOn(true);
    message = HC12.readStringUntil('\n');
    // Serial.println("Time: " + String(millis() - now) + " ms");
    Serial.println("Received: " + message);
    // const char *msg = message.c_str();
    // Serial.println(msg + 1); // skip first character
    if (millis() - now < 100)
      delay(100);
    ledOn(false);
  }
}
