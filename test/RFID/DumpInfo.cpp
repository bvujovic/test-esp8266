//* lib_deps = miguelbalboa/MFRC522@^1.4.11

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN D3 // Configurable, see typical pin layout above
#define SS_PIN D8  // Configurable, see typical pin layout above
//* SPI MOSI	D7
//* SPI MISO	D6
//* SPI SCK     D5

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance
const byte pinLed = LED_BUILTIN;

void setup()
{
    pinMode(pinLed, OUTPUT);
    digitalWrite(pinLed, true);
    Serial.begin(115200); // Initialize serial communications with the PC
    while (!Serial)
        ;                              // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
    SPI.begin();                       // Init SPI bus
    mfrc522.PCD_Init();                // Init MFRC522
    delay(100);                        // Optional delay. Some board do need more time after init to be ready, see Readme
    mfrc522.PCD_DumpVersionToSerial(); // Show details of PCD - MFRC522 Card Reader details
    Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop()
{
    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    if (!mfrc522.PICC_IsNewCardPresent())
        return;

    // Select one of the cards
    if (!mfrc522.PICC_ReadCardSerial())
        return;

    digitalWrite(pinLed, false);
    // Dump debug info about the card; PICC_HaltA() is automatically called
    mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
    digitalWrite(pinLed, true);
}
