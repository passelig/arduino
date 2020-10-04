#include <SoftwareSerial.h>

//Create software serial object to communicate with SIM800L
SoftwareSerial simSerial(3, 2); //SIM800L Tx & Rx is connected to Arduino #3 & #2
String sms = "";
//bool initComplete = false;
String inverterStatus = "off";
String phoneNumber = "+4747249027"; 

void setup()
{
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  
  //Begin serial communication with Arduino and SIM800L
  simSerial.begin(9600);
  Serial.println("Initializing... waiting 10 sec for sim to register on network"); 
  delay(1000);
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
  //initComplete = true;
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
      sendStatusSms("67","67","36.97",inverterStatus);
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
