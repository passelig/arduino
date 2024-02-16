#include <util/atomic.h>  // For the ATOMIC_BLOCK macro

const int FwdPin = 10;             //Forward Motor Pin
const int BwdPin = 9;              //Backward Motor Pin
const int acceptedDeviation = 10;  //Backward Motor Pin
int pwmSpeed = 255;

const int encoderPinA = 2;
const int encoderPinB = 3;

volatile int position = 0;  // specify position as volatile
int printPosition = 0;
int receivedInteger = 0;
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
    printPosition = position;
  }

  if (Serial.available() > 0) {
    // Read the incoming integer
    receivedInteger = Serial.parseInt();
  }
  if (currentMillis - previousMillis >= 1000) {
    Serial.println(String(printPosition) + " " + String(receivedInteger));
    // Save the current time for the next iteration
    previousMillis = currentMillis;
  }
  setForwardOrBackward();
}

void setForwardOrBackward() {
  if (abs(printPosition - receivedInteger) < acceptedDeviation) {
    analogWrite(BwdPin, 0);  //Stop
    analogWrite(FwdPin, 0);
    return;
  }
  if (printPosition < receivedInteger) {
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
    position++;
  } else {
    position--;
  }
}