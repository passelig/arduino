unsigned long startTime;
unsigned long endTime;
//String testData = "Hello world. If this string is longer than 64 bytes, the serial buffer fills up and the prosessor has to wait for it to be emptied";
String testData = "This string is shorter than 64 bytes";

void setup() {
  // Initialize serial communication at 9600 baud
  Serial.begin(9600);

  // Wait for the serial port to initialize (for some boards like Leonardo)
  while (!Serial) {
    ; // Wait for serial port to connect.
  }

  Serial.println("Testing Serial.print()...");
  
  // Capture the start time
  startTime = micros();  // micros() gives time in microseconds
  
  // Send the test string via Serial.print()
  Serial.print(testData);
  
  // Capture the end time after the data has been sent
  endTime = micros();
  
  // Calculate the time difference
  unsigned long duration = endTime - startTime;
  
  // Print the time it took to send the string
  Serial.println();
  Serial.print("Time taken to send data: ");
  Serial.print(duration);
  Serial.println(" microseconds");
}

void loop() {
  // Nothing to do in loop
}
