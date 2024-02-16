/*
  Analog input, analog output, serial output
*/

// These constants won't change. They're used to give names to the pins used:
const int analogInPin = A0;  // Analog input connected to the voltage devider
int sensorValue = 0;  // value read from the voltage devider
float calculatedTemperature = 0;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}

void loop() {
  // read the analog in value:
sensorValue = analogRead(analogInPin);
  // map it to the range of the analog out:
  //calculatedTemperature = map(sensorValue, 510, 610, 11, 20);
  calculatedTemperature = float(map(sensorValue, 3, 8, 100, 180)) / 10;
 
  // print the results to the Serial Monitor:
  //Serial.print("Value from analog to digital converter = ");
  Serial.print(String(sensorValue) + " ");
  Serial.println(String(calculatedTemperature));

  // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  delay(1000);
}
