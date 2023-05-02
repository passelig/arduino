#include <Servo.h>
int trigPin = 6;
int echoPin = 7;
int pingTravelTime;
float distance;

 

int positionOpen = 90;
int positionClosed = 0;

Servo barrier;



void setup()
{
pinMode(trigPin,OUTPUT);
pinMode(echoPin,INPUT);
Serial.begin(9600);

barrier.attach(9, 500, 2500);
barrier.write(positionOpen);
delay(500);
}


 

void loop() {
 
 int requestFromBarrier = roadBarrierRequest();
 Serial.println("Request from barrier = " + String (requestFromBarrier));

 if (requestFromBarrier == 0) {
   barrier.write(positionClosed);
   Serial.println("Barrier close");
  }
  
 if (requestFromBarrier == 1) {
   barrier.write(positionOpen);
   Serial.println("Barrier open");
  }
 delay(1000);

}




int roadBarrierRequest() {

 float distanceFromSensor = readDistanceFromSensor ();
 byte barrierRequest;

 if (distanceFromSensor > 10) {
   barrierRequest = 0; 
  }

 if (distanceFromSensor < 10) {
   barrierRequest = 1; 
  }
 
 return (barrierRequest);

}




float readDistanceFromSensor () {

 digitalWrite(trigPin,LOW);
 delayMicroseconds(10);
 digitalWrite(trigPin,HIGH);
 delayMicroseconds(10);
 digitalWrite(trigPin,LOW);
 pingTravelTime = pulseIn(echoPin,HIGH);

 //Serial.print("ping = ");
 //Serial.print(pingTravelTime);
 distance = ((pingTravelTime-200.)/60.)+1;
 Serial.println(" Distance = " + String(distance));

 return(distance);

}
