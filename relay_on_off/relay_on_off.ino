const int inverter = 12;
const int auxillary = 13;
const int on = 0;
const int off = 1;

void setup() {
  pinMode(auxillary, OUTPUT); 
  pinMode(inverter, OUTPUT); 
  Serial.print("initialization complete...");

}

void loop() {
  digitalWrite(auxillary, on);
  delay(1000);
  digitalWrite(auxillary, off);
  delay(1000);
  digitalWrite(inverter, on);
  delay(1000);
  digitalWrite(inverter, off);
  delay(1000);
}
