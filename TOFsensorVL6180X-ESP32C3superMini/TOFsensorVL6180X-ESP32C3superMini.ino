#include <Wire.h>
#include "Adafruit_VL6180X.h"

#define LED_PIN 8  // Change to 2 if your board uses GPIO2 for the onboard LED

Adafruit_VL6180X vl = Adafruit_VL6180X();

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  // Start with LED off

  Wire.begin(6, 7);  // Default I2C pins on ESP32-C3 Super Mini (SDA=6, SCL=7)

  if (!vl.begin()) {
    Serial.println("Failed to find VL6180X sensor. Check wiring!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("VL6180X Ready!");
}

void loop() {
  uint8_t range = vl.readRange();
  uint8_t status = vl.readRangeStatus();

  if (status == VL6180X_ERROR_NONE) {
    Serial.print("Distance: ");
    Serial.print(range);
    Serial.println(" mm");

    if (range < 50) {
      digitalWrite(LED_PIN, HIGH);  // Object is close, turn LED on
    } else {
      digitalWrite(LED_PIN, LOW);   // Object is far, turn LED off
    }
  } else {
    Serial.print("Range error: ");
    Serial.println(status);
  }

  delay(200);  // Short delay to avoid spamming
}
