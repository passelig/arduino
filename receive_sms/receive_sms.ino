#include <SoftwareSerial.h>

//Create software serial object to communicate with SIM800L
SoftwareSerial simSerial(3, 2); //SIM800L Tx & Rx is connected to Arduino #3 & #2
String sms = "";
bool initComplete = false;
int ledState = LOW; 

void setup()
{
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  
  //Begin serial communication with Arduino and SIM800L
  simSerial.begin(9600);
  Serial.println("Initializing..."); 
  delay(1000);
  simSerial.println("AT"); //Once the handshake test is successful, it will back to OK
  simSerial.println("AT+CMGF=1"); // Configuring TEXT mode
  simSerial.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS messages should be handled
  pinMode(LED_BUILTIN, OUTPUT); 
}

void loop()
{
  initComplete = true;
  updateSerial(); 
  if (sms.length() > 1) {
    Serial.print(sms);
     if (ledState == HIGH) {
      digitalWrite(LED_BUILTIN, LOW);
      ledState = LOW;
    } else {
      digitalWrite(LED_BUILTIN, HIGH);
      ledState = HIGH;
    }
    Serial.print(sms.length());
    sms = "";
  }
}

void updateSerial()
{
  delay(500);
  while(simSerial.available()) 
  {
    char c = simSerial.read();
    if (initComplete){ 
      sms += c;
    }
  }
}
