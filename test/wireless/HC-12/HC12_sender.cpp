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

#include <SoftwareSerial.h>
const byte pinRx = D5; // HC-12 RX Pin
const byte pinTx = D7; // HC-12 TX Pin
SoftwareSerial HC12(pinRx, pinTx);

const byte pinLed = LED_BUILTIN;

void setup()
{
  Serial.begin(115200); // Serial port to computer
  // HC12.begin(9600);     // Serial port to HC12
  HC12.begin(4800);   // Serial port to HC12
  Serial.println("\nHC-12 Sender Test");
  
  pinMode(pinLed, OUTPUT);
  // digitalWrite(pinLed, LOW);
}

int i = 0;

void loop()
{
  digitalWrite(pinLed, LOW);
  Serial.println(i);
  HC12.write(String(i++).c_str());
  digitalWrite(pinLed, HIGH);
  delay(i % 5 == 0 ? 5000 : 2000);
}
