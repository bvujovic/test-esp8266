//* platformio.ini: lib_deps = olikraus/U8g2@^2.35.15

#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
U8G2_SSD1306_64X48_ER_1_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE); // EastRising 0.66" OLED breakout board, Uno: A4=SDA, A5=SCL, 5V powered

// #include "ClickButton.h"
// const byte pinBtnLeft = D7;
// const byte pinBtnCenter = D8;
// const byte pinBtnRight = D3; //! samo D3 moze da koristi interni PULLUP
// ClickButton btnLeft(pinBtnLeft, LOW, CLICKBTN_PULLUP);
// ClickButton btnCenter(pinBtnCenter, HIGH);
// ClickButton btnRight(pinBtnRight, LOW, CLICKBTN_PULLUP);

ulong itvDelay = 1000;

const byte pinLed = LED_BUILTIN;

void blink()
{
    Serial.println("blink");
    // digitalWrite(pinLed, false);
    // delay(333);
    // digitalWrite(pinLed, true);
}

void setup()
{
    Serial.begin(115200);
    pinMode(pinLed, OUTPUT);
    digitalWrite(pinLed, true);
    blink();
    // pinMode(pinBtnLeft, INPUT_PULLUP);
    u8g2.begin();
    // btnLeft.multiclickTime = 500;
    // btnCenter.multiclickTime = 500;
    // btnRight.multiclickTime = 500;
}

uint8_t m = 15;
ulong msLastTick = millis();

void loop()
{
    delay(10);
    ulong ms = millis();

    // btnLeft.Update();
    // btnCenter.Update();
    // btnRight.Update();

    // if(!digitalRead(pinBtnLeft))
    //     Serial.print("x");

    // if (btnLeft.clicks == 1)
    //     itvDelay /= 2;
    // if (btnCenter.clicks == 1)
    //     itvDelay = 1000;
    // if (btnRight.clicks == 1)
    //     itvDelay *= 2;

    // if (btnRight.clicks != 0 || btnCenter.clicks != 0)
    //     blink();

    // if (btnSS.clicks == -1) // dugi klik
    // if (btnSS.clicks == 2) // dvostruki klik
    // if (tim.refresh(ms)) // refresh displeja i pustanje zvuka ako je doslo vreme za to
    //     buzz.blink();

    if (ms > msLastTick + itvDelay)
    {
        char m_str[3];
        strcpy(m_str, u8x8_u8toa(m, 2)); /* convert m to a string with two digits */
        u8g2.firstPage();
        u8g2.setFont(u8g2_font_logisoso24_tn);
        do
        {
            u8g2.drawStr(0, 40, "06");
            u8g2.drawStr(29, 40, ":");
            u8g2.drawStr(35, 40, m_str);
        } while (u8g2.nextPage());
        // B delay(m % 10 == 0 ? 5000 : 1000);
        m++;
        if (m == 60)
            m = 0;
        msLastTick = ms;
    }
}
