#include <util/atomic.h> // For the ATOMIC_BLOCK macro

const int encoderPinA = 2; 
const int encoderPinB = 3;  

volatile int position = 0; // specify position as volatile
int printPosition= 0; 

void setup() {
  Serial.begin(9600);
  pinMode(encoderPinA,INPUT);
  pinMode(encoderPinB,INPUT);
  attachInterrupt(digitalPinToInterrupt(encoderPinA),readEncoder,RISING);
}

void loop() {
  // Read the positiontion in an atomic block to avoid a potential
  // misread if the interrupt coincides with this code running
  // see: https://www.arduino.cc/reference/en/language/variables/variable-scope-qualifiers/volatile/

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    printPosition= position;
  }
  
  Serial.println(printPosition);
  delay(100);

}

void readEncoder(){
  int encoderB = digitalRead(encoderPinB);
  if(encoderB > 0){
    position++;
  }
  else{
    position--;
  }
}