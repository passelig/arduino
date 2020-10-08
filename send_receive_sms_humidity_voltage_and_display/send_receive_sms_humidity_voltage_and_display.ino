#include <SoftwareSerial.h>
#include <Nextion.h>
#include <Wire.h>
#include "ClosedCube_HDC1080.h"

//Create software serial object to communicate with SIM800L
SoftwareSerial simSerial(3, 2); //SIM800L Tx & Rx is connected to Arduino #3 & #2
String sms = "";
//bool initComplete = false;
String inverterStatus = "off";
String phoneNumber = "+4747249027"; 
ClosedCube_HDC1080 hdc1080;
int rx_pin = 4;
int tx_pin = 5;
int sensorPin = A0;  
int batteryPercent = 0;
int voltageSensorValue = 0;
float voltage = 12.3;
int humidity = 10;
String voltageString = "7777";
float voltageEmpty = 32.0;
float voltageFull = 41.9;
Nextion nextion(rx_pin, tx_pin);
const int numReadings = 10;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average


void setup()
{
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  nextion.begin(9600);
  hdc1080.begin(0x40);
  
  //Begin serial communication with Arduino and SIM800L
  simSerial.begin(9600);
  Serial.println("Initializing... waiting 10 sec for sim to register on network"); 
  delay(10000);
  simSerial.println("AT"); //Once the handshake test is successful, it will back to OK
  Serial.print("1: " + readSimSerial());
  simSerial.println("AT+CMGF=1"); // Configuring TEXT mode
  Serial.print("2: " + readSimSerial());
  simSerial.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS messages should be handled
  Serial.println("3: " + readSimSerial());
  Serial.println("initialization complete...");
}

void loop()
{
  voltageSensorValue = smoothenReadings(analogRead(sensorPin));
  Serial.println(voltageSensorValue);
  voltage = voltageSensorValue * 0.05617;
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
  
  sms = readSimSerial(); 
  if (sms.length() > 1) {
    sms.toLowerCase();
    Serial.print(sms);
    Serial.print(sms.length());
    if (sms.length() > 40){
      if (sms.indexOf("inverter off") > 0) {
        //Serial.print("Turn it off");
        digitalWrite(LED_BUILTIN, LOW);
        inverterStatus = "off";
      } 
      if (sms.indexOf("inverter on") > 0) {
        digitalWrite(LED_BUILTIN, HIGH);
        inverterStatus = "on";
      }
      sendStatusSms(String(humidity),String(batteryPercent),String(voltage),inverterStatus);
    }
   sms = "";
  }   
}

void sendStatusSms (String humidity, String chargePercent, String batteryVoltage, String inverterStatus){
  Serial.print("Sending status SMS...");
  simSerial.println("AT+CMGF=1"); // Configuring TEXT mode
  Serial.println("6: " + readSimSerial());
  simSerial.println("AT+CMGS=\"" + phoneNumber + "\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  simSerial.println("Status at Lille Buddha:");
  simSerial.println("Humidity: " + humidity + " %");//text content
  simSerial.println("Charge: " + chargePercent + "% (" + batteryVoltage + "v)");//text content
  simSerial.print("Inverter: " + inverterStatus);
  Serial.println("7: " + readSimSerial());
  simSerial.write(26);
}

String readSimSerial()
{
  String message ="";
  delay(500);
  while(simSerial.available()) 
  {
    char c = simSerial.read();
    //if (initComplete){ 
      message += c;
    //}
  }
  message.toLowerCase();
  return message;
}

int smoothenReadings(int reading){
 // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = reading;
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;
  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }
  // calculate the average:
  average = total / numReadings;
  // send it to the computer as ASCII digits
  delay(1);        // delay in between reads for stability
  return average;
}
