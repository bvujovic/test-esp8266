//* Test za 433Mhz HC-12 SI4463 module. Oba skeca su testirana na Arduinima.
//* https://www.youtube.com/watch?v=vqRqtgvltOI
//* https://www.youtube.com/watch?v=_CmZsuGwsfg
//* https://www.datsi.fi.upm.es/docencia/DMC/HC-12_v2.3A.pdf
//* https://wolles-elektronikkiste.de/en/hc-12-radio-module

//* SENDER
// 10k otpornik izmedju VCC i SET pinova za slanje podataka
// #include <SoftwareSerial.h>
// SoftwareSerial HC12(10, 11); // HC-12 TX Pin, HC-12 RX Pin
//
// void setup() {
//  HC12.begin(9600);               // Serial port to HC12
//}
//
// void loop() {
//  delay(2000);
//  HC12.write("Test 123");
//}

//* RECEIVER
#include <SoftwareSerial.h>

SoftwareSerial HC12(10, 11); // HC-12 TX Pin, HC-12 RX Pin

void setup()
{
  Serial.begin(9600); // Serial port to computer
  HC12.begin(9600);   // Serial port to HC12
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);
}

void loop()
{
  while (HC12.available())
  {                            // If HC-12 has data
    Serial.write(HC12.read()); // Send the data to Serial monitor
  }
  while (Serial.available())
  {                            // If Serial monitor has data
    HC12.write(Serial.read()); // Send that data to HC-12
  }
}
