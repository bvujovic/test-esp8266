#include <Arduino.h>
#include <TM1637Display.h> // lib_deps = smougenot/TM1637@0.0.0-alpha+sha.9486982048

#define CLK D2
#define DIO D3

TM1637Display display(CLK, DIO);

void setup()
{
    display.setBrightness(5);
    display.showNumberDec(1234);
}

void loop()
{
    delay(1000);
}
