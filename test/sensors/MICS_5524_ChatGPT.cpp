//* ChatGPT test sketch; no library; NOT working with my MICS-5524 module yet

#include <Arduino.h>

#define MICS5524_PIN A0
// #define RL_VALUE 4700 // Load resistor value in ohms (4.7k typical)
#define RL_VALUE 10000 // The right resistor value for my MICS-5524 

float R0 = 0; // Sensor resistance in clean air (to be calibrated)

void setup() {
  Serial.begin(115200);
  pinMode(MICS5524_PIN, INPUT);
  Serial.println("MICS-5524 Calibration and CO Measurement");
  delay(2000);
}

float readSensorResistance() {
  int adcValue = analogRead(MICS5524_PIN);
  float voltage = adcValue * (3.3 / 1023.0);
  float Rs = (3.3 - voltage) * RL_VALUE / voltage;
  return Rs;
}

void loop() {
  // Step 1: Calibrate in clean air (run this and note the R0 value)
  if (R0 == 0) {
    float sum = 0;
    int samples = 50;
    for (int i = 0; i < samples; i++) {
      sum += readSensorResistance();
      delay(100);
    }
    R0 = sum / samples;
    Serial.print("Calibration complete. R0 (clean air) = ");
    Serial.print(R0);
    Serial.println(" ohms");
    Serial.println("Copy this value to the R0 variable for future use.");
    delay(10000); // Wait before next reading
    return;
  }

  // Step 2: Measure Rs and estimate CO concentration
  float Rs = readSensorResistance();
  float ratio = Rs / R0;

  // Estimate CO concentration using datasheet curve (approximate)
  // For CO: log10(ppm) = (log10(ratio) - b) / m
  // Example values (from datasheet graph): m ≈ -0.77, b ≈ 0.36
  float m = -0.77;
  float b = 0.36;
  float logRatio = log10(ratio);
  float logPPM = (logRatio - b) / m;
  float ppm = pow(10, logPPM);

  Serial.print("Rs = ");
  Serial.print(Rs);
  Serial.print(" ohms | Rs/R0 = ");
  Serial.print(ratio, 2);
  Serial.print(" | CO ≈ ");
  Serial.print(ppm, 1);
  Serial.println(" ppm");

  delay(2000);
}
