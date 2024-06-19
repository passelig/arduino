#include <util/atomic.h>

const int FwdPin = 10;             // Forward Motor Pin
const int BwdPin = 9;              // Backward Motor Pin
const int acceptedDeviation = 12;   // Accepted deviation for stopping the motor

const int encoderPinA = 2;
const int encoderPinB = 3;

volatile long interruptPosition = 0;  // Specify position as volatile
long position = 0;
long setpoint = 0;
unsigned long currentMillis, previousMillis = 0;
bool newSetpointReceived = false;

void setup() {
  Serial.begin(115200);
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  pinMode(FwdPin, OUTPUT);
  pinMode(BwdPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(encoderPinA), readEncoder, RISING);
}

void loop() {
  currentMillis = millis();
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    position = interruptPosition;
  }

  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    if (input.length() > 0) {
      setpoint = input.toInt();
      newSetpointReceived = true;
      Serial.print("Setpoint: ");
      Serial.println(setpoint);
    }
  }

  if (currentMillis - previousMillis >= 70) {
    Serial.print("Position: ");
    Serial.print(position);
    Serial.print(" Setpoint: ");
    Serial.print(setpoint);
    Serial.print(" Deviation: ");
    Serial.println(abs(position - setpoint));
    previousMillis = currentMillis;
  }

  if (newSetpointReceived) {
    setForwardOrBackward();
  }
}

void setForwardOrBackward() {
  int deviation = abs(position - setpoint);
  if (deviation < acceptedDeviation) {
    digitalWrite(BwdPin, LOW);  // Stop
    digitalWrite(FwdPin, LOW);
  } else if (position < setpoint) {
    digitalWrite(FwdPin, HIGH);  // Move Forward
    digitalWrite(BwdPin, LOW);
  } else {
    digitalWrite(BwdPin, HIGH);  // Move Backward
    digitalWrite(FwdPin, LOW);
  }
}

void readEncoder() {
  int encoderB = digitalRead(encoderPinB);
  if (encoderB > 0) {
    interruptPosition++;
  } else {
    interruptPosition--;
  }
}
