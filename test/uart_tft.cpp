//* TZT 2.4 Inch TFT 240*320 Resolution 3.3V-5V UART MCU Serial Communication
//* https://www.aliexpress.com/item/1005004663674198.htm
//* display TX - NodeMCU D4 (Serial1)


#include <Arduino.h>

// String resp;

void setup()
{
    delay(1000);
    Serial1.begin(115200);

    // Serial1.println("VER;");
    // delay(1000);
    // resp = Serial1.readString();
    // Serial.printf("Response: %s\n", resp.c_str());
    // delay(1000);

    // Serial1.println("MODE_CFG(0);");
    // delay(1000);

    Serial1.println("BPS(115200);");
    delay(1000);

    Serial1.println("CLR(1);");
    delay(1000);

    Serial1.println("BL(0);");
    delay(1500);

    Serial1.println("BL(127);");
    delay(1500);

    Serial1.println("CLR(0);");
    delay(1000);

    Serial1.println("DC32(10,10,'Nikola Jokic #15.',18);");
    Serial1.println("DC32(10,50,'28*C-+=!@~$&*?/^\\',4);");
    Serial1.println("DC32(10,90,'<> () {} [] ;',5);");
    delay(1000);

    Serial1.println("CIRF(25,200,24,1);");
    delay(1000);
    Serial1.println("CIRF(75,200,24,2);");
    delay(1000);
    Serial1.println("CIRF(125,200,24,3);");
    delay(10 * 1000);
    Serial1.println("CIRF(175,200,24,4);");
    delay(60 * 1000);
    Serial1.println("CIRF(225,200,24,5);");
    delay(600 * 1000);
    Serial1.println("CIRF(275,200,24,6);");
}

void loop()
{
}