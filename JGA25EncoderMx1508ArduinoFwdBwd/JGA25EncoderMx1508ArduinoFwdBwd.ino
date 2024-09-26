// Define control pins
const int IN1 = 9; // Control pin 1
const int IN2 = 10; // Control pin 2

// Define PWM speed (70% of 255)
const int speed = 255;

void setup() {
  // Set control pins as outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
}

void loop() {
  // Run the motor forward at 70% speed
  analogWrite(IN1, speed); // Set IN1 to 70% PWM
  analogWrite(IN2, 0);     // Set IN2 to 0
  delay(2000); // Run for 2 seconds

  // Stop the motor
  analogWrite(IN1, 0); // Set both IN1 and IN2 to 0 to stop
  analogWrite(IN2, 0);
  delay(1000); // Stop for 1 second

  // Run the motor backward at 70% speed
  analogWrite(IN1, 0);     // Set IN1 to 0
  analogWrite(IN2, speed); // Set IN2 to 70% PWM
  delay(2000); // Run for 2 seconds

  // Stop the motor
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  delay(1000); // Stop for 1 second
}
