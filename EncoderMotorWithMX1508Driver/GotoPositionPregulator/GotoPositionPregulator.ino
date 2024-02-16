#include <util/atomic.h>  // For the ATOMIC_BLOCK macro

const int FwdPin = 10;             //Forward Motor Pin
const int BwdPin = 9;              //Backward Motor Pin
const int acceptedDeviation = 10;  //Backward Motor Pin
int pwmSpeed = 255;
int deviation = 0;

const int encoderPinA = 2;
const int encoderPinB = 3;

volatile int interuptPosition = 0;  // specify position as volatile
int position = 0;
int setpoint = 0;
unsigned long currentMillis, previousMillis = 0;

void setup() {
  Serial.begin(9600);
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  pinMode(FwdPin, OUTPUT);
  pinMode(BwdPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(encoderPinA), readEncoder, RISING);
}

void loop() {
  // Read the positiontion in an atomic block to avoid a potential
  // misread if the interrupt coincides with this code running
  // see: https://www.arduino.cc/reference/en/language/variables/variable-scope-qualifiers/volatile/
  currentMillis = millis();
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    position = interuptPosition;
  }

  if (Serial.available() > 0) {
    // Read the incoming integer
    setpoint = Serial.parseInt();
  }
  if (currentMillis - previousMillis >= 10) {
    if (deviation > 10) {
      Serial.println(String(position) + " " + String(setpoint) + " " + String(pwmSpeed));
    }
    // Save the current time for the next iteration
    previousMillis = currentMillis;
  }
  setForwardOrBackward();
}

void setForwardOrBackward() {
  deviation = abs(position - setpoint);
  if (deviation < acceptedDeviation) {
    analogWrite(BwdPin, 0);  //Stop
    analogWrite(FwdPin, 0);
    return;
  }
  pwmSpeed = constrain(deviation, 150, 255);
  if (position < setpoint) {
    analogWrite(FwdPin, pwmSpeed);  //Send instructions to Forward motor pin
    analogWrite(BwdPin, 0);
  } else {
    analogWrite(BwdPin, pwmSpeed);  //Send instructions to Forward motor pin
    analogWrite(FwdPin, 0);
  }
}

void readEncoder() {
  int encoderB = digitalRead(encoderPinB);
  if (encoderB > 0) {
    interuptPosition++;
  } else {
    interuptPosition--;
  }
}