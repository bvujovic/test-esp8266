//* Test for 433Mhz HC-12 module. Sketch forwards data between HC-12 and Serial monitor.
//* For sending AT commands to HC-12, connect SET pin to GND.
//* https://www.youtube.com/watch?v=vqRqtgvltOI
//* https://www.youtube.com/watch?v=_CmZsuGwsfg
//* https://www.datsi.fi.upm.es/docencia/DMC/HC-12_v2.3A.pdf
//* https://wolles-elektronikkiste.de/en/hc-12-radio-module

#include <SoftwareSerial.h>

const byte pinRx = D5; // HC-12 RX Pin
const byte pinTx = D7; // HC-12 TX Pin
SoftwareSerial HC12(pinRx, pinTx);

void setup()
{
  Serial.begin(115200); // Serial port to computer
  Serial.println("\nHC-12 Serial to Serial Test");
  HC12.begin(9600); // Serial port to HC12
  // HC12.begin(4800); // Serial port to HC12
}

void loop()
{
  while (HC12.available())     // While HC-12 has data...
    Serial.write(HC12.read()); // ...send the data to Serial monitor
  while (Serial.available())   // While Serial monitor has data...
    HC12.write(Serial.read()); // ...send that data to HC-12
}

// cmd("AT+FU2");   // Set to FU2 mode
// cmd("AT+P4");    // Set power to 4: 8dBm; Set power to 2: 2dBm
// cmd("AT+C030");  // Set channel to 30
// cmd("AT+RX");
