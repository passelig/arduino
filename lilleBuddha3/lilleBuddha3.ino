
#include <Nextion.h>
#include <Wire.h>
#include "ClosedCube_HDC1080.h"
ClosedCube_HDC1080 hdc1080;
// connect SDA to A4 on arduino and SCL to A5 

int rx_pin = 4;
int tx_pin = 5;
int sensorPin = A0;  
int percent = 0;
int sensorValue = 0;
String humidity = "";

Nextion nextion(rx_pin, tx_pin);

void setup() {
  nextion.begin(9600);
  Serial.begin(9600);
  hdc1080.begin(0x40);
  Serial.println("Arduiono started");
}

void loop() {
  //sensorValue = analogRead(sensorPin);
  //Serial.println(sensorValue);
  //percent = sensorValue/8.1;
  percent = 33;
  nextion.print("batteryPercent.val=" + String(percent));
  nextion.print("p1.pic=" + String(percent/5+1));
  humidity = String(hdc1080.readHumidity());
  Serial.println(humidity.substring(0,2)); 
  nextion.print("humidity.val=" + humidity.substring(0,2));
  delay(1000);
  
}
