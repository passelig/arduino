
#include <Nextion.h>

int rx_pin = 4;
int tx_pin = 5;
int sensorPin = A0;  
int percent = 0;
int sensorValue = 0;

Nextion nextion(rx_pin, tx_pin);

void setup() {
  nextion.begin(9600);
  Serial.begin(9600);
  Serial.println("Arduiono started");
}

void loop() {
  //sensorValue = analogRead(sensorPin);
  //Serial.println(sensorValue);
  //percent = sensorValue/8.1;
  percent = 33;
  nextion.print("batteryPercent.val=" + String(percent));
  nextion.print("p1.pic=" + String(percent/5+1));

  nextion.print("humidity.val=78");
  delay(10000);
  
}
