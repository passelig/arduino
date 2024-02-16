
const int FwdPin = 10; //Forward Motor Pin
const int BwdPin = 9;  //Backward Motor Pin
int pwmSpeed = 255;


void setup()
{
  pinMode(FwdPin, OUTPUT);
  pinMode(BwdPin, OUTPUT);
}

void loop()
{
    analogWrite(FwdPin,pwmSpeed);  //Send instructions to Forward motor pin
    analogWrite(BwdPin,0);
    delay(3000);
    analogWrite(BwdPin,pwmSpeed);  //Send instructions to Forward motor pin
    analogWrite(FwdPin,0);
    delay(3000);
}
