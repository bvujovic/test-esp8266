//* Example code to drive a 3-digit 7-segment display using 3 cascaded 74HC595 shift registers
//* Displayed numbers start from 120 and increment every second
//* Decimal point cycles through positions 1 to 3 but is off at 0
//* Display draws about 30mA.

#include <Arduino.h>

// Pin definitions for 74HC595
const int dataPin = D5;  // DS
const int clockPin = D6; // SHCP
const int latchPin = D7; // STCP

// 7-segment encoding for digits 0-9 (abcdefg, active HIGH)
const byte digitSegments[10] = {
    B11000000, // 0
    B11111001, // 1
    B10100100, // 2
    B10110000, // 3
    B10011001, // 4
    B10010010, // 5
    B10000010, // 6
    B11111000, // 7
    B10000000, // 8
    B10010000  // 9
};

void setup()
{
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(latchPin, OUTPUT);
}

const byte dotValue = B10000000; // Decimal point segment

void displayNumber(int number, int dotPos)
{
    // Clamp to 0-999
    number = constrain(number, 0, 999);

    // Split into digits
    int hundreds = number / 100;
    int tens = (number / 10) % 10;
    int ones = number % 10;

    // Prepare segment data for 3 digits
    byte segments[3] = {
        digitSegments[ones],
        digitSegments[tens],
        digitSegments[hundreds]};
    if (dotPos > 0 && dotPos < 4)
        segments[3 - dotPos] -= dotValue; // Add decimal point to the correct digit

    // Send data to 3 shift registers (MSB first)
    digitalWrite(latchPin, LOW);
    for (int i = 0; i < 3; i++)
        shiftOut(dataPin, clockPin, MSBFIRST, segments[i]);
    digitalWrite(latchPin, HIGH);
}

void loop()
{
    static int num = 120;  // Starting number
    static int dotPos = 0; // Position of decimal point (1-3), 0 for none
    displayNumber(num, dotPos);
    num = (num + 1) % 1000;    // Count up to 999
    dotPos = (dotPos + 1) % 4; // Cycle dot position
    delay(1000);
}