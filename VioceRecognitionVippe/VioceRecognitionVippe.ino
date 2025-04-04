/*!
 * @file  i2c.ino
 * @brief Control the voice recognition module via I2C
 * @n  Get the recognized command ID and play the corresponding reply audio according to the ID;
 * @n  Get and set the wake-up state duration
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence  The MIT License (MIT)
 * @author  [qsjhyy](yihuan.huang@dfrobot.com)
 * @version  V1.0
 * @date  2022-04-02
 * @url  https://github.com/DFRobot/DFRobot_DF2301Q
 */
#include "DFRobot_DF2301Q.h"
#include <Servo.h>

Servo myServo;           // Create a servo object
const int servoPin = 9;  // Define the pin for the servo signal

// Servo angle limits
const int minAngle = 0;
const int maxAngle = 180;
const int midAngle = (minAngle + maxAngle) / 2;

#define Led 13

//I2C communication
DFRobot_DF2301Q_I2C asr;

void setup() {
  Serial.begin(115200);

  pinMode(Led, OUTPUT);      //Init LED pin to output mode
  digitalWrite(Led, LOW);    //Set LED pin to low
  myServo.attach(servoPin);  // Attach the servo to the defined pin

  Serial.println("Servo control Enabled");

  // Init the sensor
  while (!(asr.begin())) {
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }
  Serial.println("Begin ok!");

  /**
   * @brief Set voice volume
   * @param voc - Volume value(1~7)
   */
  asr.setVolume(4);

  /**
     @brief Set mute mode
     @param mode - Mute mode; set value 1: mute, 0: unmute
  */
  asr.setMuteMode(0);

  /**
     @brief Set wake-up duration
     @param wakeTime - Wake-up duration (0-255)
  */
  asr.setWakeTime(20);

  /**
     @brief Get wake-up duration
     @return The currently-set wake-up period
  */
  uint8_t wakeTime = 0;
  wakeTime = asr.getWakeTime();
  Serial.print("wakeTime = ");
  Serial.println(wakeTime);

  // asr.playByCMDID(1);   // Wake-up command

  /**
     @brief Play the corresponding reply audio according to the ID
     @param CMDID - command word ID
  */
  //asr.playByCMDID(23);  // Command word ID
}

void loop() {
  /**
     @brief Get the ID corresponding to the command word 
     @return Return the obtained command word ID, returning 0 means no valid ID is obtained
  */
  uint8_t CMDID = asr.getCMDID();
  switch (CMDID) {
    case 103:                                                           //If the command is “Turn on the light”
      digitalWrite(Led, HIGH);                                          //Turn on the LED
      Serial.println("received'Turn on the light',command flag'103'");  //Serial transmits "received"Turn on the light",command flag"103
      break;

    case 104:                                                            //If the command is “Turn off the light”
      digitalWrite(Led, LOW);                                            //Turn off the LED
      Serial.println("received'Turn off the light',command flag'104'");  //The serial transmits "received"Turn off the light",command flag"104""
      break;

    case 5:  //Vipp til venstre
      Serial.println("Vipper til venstre");
      myServo.write(maxAngle);
      break;

    case 6:  //Vipp til venstre
      Serial.println("Vipper til høyre");
      myServo.write(minAngle);
      break;

    case 7:  //Vipp til venstre
      Serial.println("Går til midtstilling");
      myServo.write(midAngle);
      break;


    default:
      if (CMDID != 0) {
        Serial.print("CMDID = ");  //Printing command ID
        Serial.println(CMDID);
      }
  }
  delay(300);
}