#include <util/atomic.h>  // For the ATOMIC_BLOCK macro
#define minimumPWM 100

const int FwdPin = 10;             //Forward Motor Pin
const int BwdPin = 9;              //Backward Motor Pin
const int acceptedDeviation = 3;  //Backward Motor Pin
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
  currentMillis = millis();
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    position = interuptPosition;
  }

  if (Serial.available() > 0) {
    // Read the incoming integer
    setpoint = Serial.parseInt();
  }
  if (currentMillis - previousMillis >= 70) {
    if (deviation > acceptedDeviation) {
      Serial.println(String(position) + " " + String(deviation) + " " + String(pwmSpeed));
    }
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
  
  // Start regulating at 500 ticks from setpoint down to "minimumPWM"
  if (deviation < 500){
    pwmSpeed = minimumPWM +(125.0/500*deviation);
  } else {
    pwmSpeed = 255;
  }
 
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