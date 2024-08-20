// Define the GPIO pins
#define LED_PIN 2      // LED connected to GPIO 2
#define BUTTON_PIN 4   // Button connected to GPIO 4

// Variable to store the LED state
bool ledState = LOW;

// Variable to store the previous button state
bool lastButtonState = HIGH;  // Start with HIGH because of pull-up

void setup() {
  // Initialize the LED pin as an output
  pinMode(LED_PIN, OUTPUT);

  // Initialize the button pin as an input with internal pull-up resistor
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Ensure LED is initially off
  digitalWrite(LED_PIN, ledState);

  // Start the serial communication for debugging
  Serial.begin(115200);
}

void loop() {
  // Read the current state of the button
  bool buttonState = digitalRead(BUTTON_PIN);

  // Print the current states as 0 or 1 to the Serial Monitor
 
  Serial.println("ledState:" + String(ledState) + " " + "buttonState:" + String(buttonState) + " " + "lastButtonState:"  + String(lastButtonState)); // Prints 0 or 1

  // Check if the button was pressed (transition from HIGH to LOW)
  if (buttonState == LOW && lastButtonState == HIGH) {
    // Toggle the LED state
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);

    // Simple debounce delay
    delay(500);  // Adjust if needed
  }
  // Save the current button state for comparison in the next loop
  lastButtonState = buttonState;

}
