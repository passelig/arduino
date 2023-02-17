#include <Servo.h>
int trigPin = 6;
int echoPin = 7;
int pingTravelTime;
float distance;

int pos = 45;

Servo servo_9;

void setup()
{ 
   pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  Serial.begin(9600);
  
  servo_9.attach(9, 500, 2500);
  servo_9.write(pos);
  delay(500);
}

void loop()
{
   digitalWrite(trigPin,LOW);
  delayMicroseconds(10);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  pingTravelTime = pulseIn(echoPin,HIGH);
  
  Serial.print("ping = ");
  Serial.print(pingTravelTime);
  distance = ((pingTravelTime-200.)/60.)+1;
  Serial.print("  Distance = ");
  Serial.println(distance);
  delay(25);
  
}