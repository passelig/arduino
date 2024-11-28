#include <util/atomic.h>
const int encoderPinA = 2;         // Encoder pin A (interrupt pin)
const int encoderPinB = 3;         // Encoder pin B
const int motorPinA = 5;           // MX1508 IN1
const int motorPinB = 6;           // MX1508 IN2
const int acceptedDeviation = 12;  // Accepted deviation for stopping the motor


volatile long interruptPosition = 0;  // Specify position as volatile
long position = 0;
long setpoint = 0;
unsigned long currentMillis, previousMillis = 0;
bool newSetpointReceived = false;

void setup() {
  Serial.begin(115200);
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  pinMode(motorPinA, OUTPUT);
  pinMode(motorPinB, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(encoderPinA), readEncoder, RISING);
  Serial.print("Enter positoin in numberr of encoder ticks:");
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
    Serial.println(position - setpoint);
    previousMillis = currentMillis;
  }

  if (newSetpointReceived) {
    setForwardOrBackward();
  }
}

void setForwardOrBackward() {
  int deviation = position - setpoint;
  if (deviation < 0) {
    //Serial.print("Forward ");
    analogWrite(motorPinA, 255);  // Move Backward
    analogWrite(motorPinB, LOW);
  } else {
    //Serial.print("Backward ");
    analogWrite(motorPinB, 255);  // Move Forward
    analogWrite(motorPinA, LOW);
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
