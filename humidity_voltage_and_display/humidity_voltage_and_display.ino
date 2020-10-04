
#include <Nextion.h>
#include <Wire.h>
#include "ClosedCube_HDC1080.h"
 
ClosedCube_HDC1080 hdc1080;
 // connect SDA to A4 on arduino and SCL to A5 

int rx_pin = 4;
int tx_pin = 5;
int sensorPin = A0;  
int batteryPercent = 0;
int voltageSensorValue = 0;
float voltage = 12.3;
int humidity = 10;
String voltageString = "7777";
float voltageEmpty = 22.0;
float voltageFull = 31.9;


Nextion nextion(rx_pin, tx_pin);

void setup() {
  nextion.begin(9600);
  Serial.begin(9600);
  Serial.println("Arduiono started");
  hdc1080.begin(0x40);
}

void loop() {
  voltageSensorValue = analogRead(sensorPin);
  Serial.println(voltageSensorValue);
  voltage = voltageSensorValue * 0.05076;
  voltageString = String(voltage);
  voltageString.replace(".","");
  Serial.println(voltageString + "v" + String(voltage));
  humidity = (int)hdc1080.readHumidity();
  Serial.println(String(humidity) + "%" );
  //percent = sensorValue/8.1;
  if (voltage < voltageEmpty) {
    batteryPercent = 0;
  } else if (voltage > voltageFull) {
    batteryPercent = 100;
  } else {
    //Serial.print((voltage - voltageEmpty)/(voltageFull - voltageEmpty) *100);
    batteryPercent = (voltage - voltageEmpty)/(voltageFull - voltageEmpty) *100;
  }
  Serial.println("Charge:" + String(batteryPercent) + "%" );
  nextion.print("batteryPercent.val=" + String(batteryPercent));
  nextion.print("p1.pic=" + String(batteryPercent/5+1));
  nextion.print("batteryVoltage.val=" + voltageString);
  nextion.print("humidity.val=" + String(humidity));
  delay(1000);
  
}
