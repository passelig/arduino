// Define the encoder pins
const int encoderPinA = 2;
const int encoderPinB = 3;

void setup() {
  // Initialize the serial communication
  Serial.begin(9600);

  // Set the encoder pins as input
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);
}

void loop() {
  // Read the digital values from the encoder pins
  int encoderValueA = digitalRead(encoderPinA);
  int encoderValueB = digitalRead(encoderPinB);
  
  // Print the encoder values to the serial monitor
  //Serial.print("Encoder A Value: ");
  Serial.print(encoderValueA);
  //Serial.print(" | Encoder B Value: ");
  Serial.print(" ");
  Serial.println(encoderValueB);
  
  delay(1); // Adjust the delay as needed
}
