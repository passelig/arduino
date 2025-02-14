// Define the pins for the motor driver
const int IN1 = 9;  // Pin for IN1 (PWM control for speed)
const int IN2 = 10; // Pin for IN2 (direction control)
int pwmValue = 200; // Default PWM value (200)

void setup() {
  // Set the motor control pins as outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Initialize Serial communication
  Serial.begin(9600);
  Serial.println("Enter a PWM value (0 to 255) to control motor speed:");

  // Set the default motor direction to forward
  digitalWrite(IN2, LOW);
}

void loop() {
  // Check if there is any data available in the Serial Monitor
  if (Serial.available() > 0) {
    // Read the incoming PWM value
    int inputValue = Serial.parseInt();

    // Validate the PWM value (should be between 0 and 255)
    if (inputValue >= 0 && inputValue <= 255) {
      pwmValue = inputValue;
      Serial.print("Setting PWM value to: ");
      Serial.println(pwmValue);
      
      // Set the motor speed using the PWM value
      analogWrite(IN1, pwmValue);
    } else {
      Serial.println("Invalid PWM value. Please enter a value between 0 and 255.");
    }
  }
}
