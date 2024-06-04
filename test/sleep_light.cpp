//* https://efcomputer.net.au/blog/esp8266-light-sleep-mode/

// TODO https://github.com/esp8266/Arduino/issues/6642

#include <Arduino.h>

// extern "C"
// {
// #include "gpio.h"
// }
extern "C"
{
#include "user_interface.h"
}

const byte pinBtn = D1;
const byte pinLed = LED_BUILTIN;

void setup()
{
    Serial.begin(74880);
    Serial.println();
    Serial.println(ESP.getResetReason());
    gpio_init(); // Initilise GPIO pins
    pinMode(pinBtn, INPUT);
    pinMode(pinLed, OUTPUT);
    digitalWrite(pinLed, HIGH);
}

void light_sleep()
{
    wifi_station_disconnect();
    wifi_set_opmode_current(NULL_MODE);
    // set sleep type, the above posters wifi_set_sleep_type() didnt seem to work for me
    // although it did let me compile and upload with no errors
    wifi_fpm_set_sleep_type(LIGHT_SLEEP_T);
    // Enables force sleep
    wifi_fpm_open();
    // GPIO_ID_PIN(2) corresponds to GPIO2 on ESP8266-01 , GPIO_PIN_INTR_LOLEVEL for a logic low,
    // can also do other interrupts, see gpio.h above
    gpio_pin_wakeup_enable(GPIO_ID_PIN(pinBtn), GPIO_PIN_INTR_HILEVEL);

    // Sleep for longest possible time
    wifi_fpm_do_sleep(0xFFFFFFF); //* THIS WORKS: doesn't sleep for 268sec, it sleeps longer (forever?)
    // wifi_fpm_do_sleep(2000000UL); //* THIS DOES NOT WORK

    delay(7000);
}

int x = 1;

void loop()
{
    x++;
    Serial.println(x);
    delay(200);
    Serial.println("Going to sleep now");
    light_sleep();
    // LED is ON for 7sec, not 0.5 (???)
    digitalWrite(pinLed, LOW);
    delay(500);
    digitalWrite(pinLed, HIGH);
    // delay(200);
    // Serial.println("Wake up");
    // Serial.println(x);
    Serial.printf("millis: %ld\n", millis());
}
