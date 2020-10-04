#include <Wire.h>
#include "ClosedCube_HDC1080.h"
 
ClosedCube_HDC1080 hdc1080;
 // connect SDA to A4 on arduino and SCL to A5 
void setup()
{
Serial.begin(9600);
hdc1080.begin(0x40);
}
 
void loop()
{

Serial.println(hdc1080.readHumidity() );

delay(3000);
}
 
