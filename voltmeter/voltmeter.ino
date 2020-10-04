int sensorPin = A0;    // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor
float gain = 0.0533;
float voltage = 0;

void setup() {
   Serial.begin(9600);
  
  Serial.println("Initializing..."); 
Serial.println("Voltmeter");
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
  voltage = gain * sensorValue;
  Serial.println(String(voltage) + "v");
  delay(500);
}
