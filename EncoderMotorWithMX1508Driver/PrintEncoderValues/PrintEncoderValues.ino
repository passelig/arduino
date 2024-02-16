
const int encoderPinA = 2; 
const int encoderPinB = 3;  
int encoderA, encoderB;


void setup()
{
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  Serial.begin(115200);
}

void loop()
{
  int encoderA = digitalRead(encoderPinA);
  int encoderB = digitalRead(encoderPinB);
  Serial.println(String(encoderA) + " "+ String(encoderB));
  delay(1); 
}
