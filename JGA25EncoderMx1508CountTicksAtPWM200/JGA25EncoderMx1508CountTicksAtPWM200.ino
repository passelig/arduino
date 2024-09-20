// Pin definitions
const int encoderPinA = 2;    // Encoder pin A (interrupt pin)
const int encoderPinB = 3;    // Encoder pin B
const int motorPinA = 9;      // MX1508 IN1
const int motorPinB = 10;     // MX1508 IN2
const int pwmSpeed = 200;     // PWM speed (0-255)

volatile long encoderCount = 0;   // Encoder count

// Variables for time tracking
unsigned long previousMillis = 0;
const long interval = 1000;       // Interval for printing (1 second)

// Interrupt service routine (ISR) for encoder
void encoderISR() {
  // Read both A and B
  int stateA = digitalRead(encoderPinA);
  int stateB = digitalRead(encoderPinB);

  // Determine direction and update the encoder count
  if (stateA == stateB) {
    encoderCount++;
  } else {
    encoderCount--;
  }
}

void setup() {
  // Initialize encoder pins
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);

  // Attach interrupt to encoderPinA
  attachInterrupt(digitalPinToInterrupt(encoderPinA), encoderISR, CHANGE);

  // Initialize motor pins
  pinMode(motorPinA, OUTPUT);
  pinMode(motorPinB, OUTPUT);

  // Start Serial communication at 115200 baud
  Serial.begin(115200);

  // Set motor speed (PWM)
  analogWrite(motorPinA, pwmSpeed);
  analogWrite(motorPinB, 0); // Set one pin low to run the motor in one direction
}

void loop() {
  unsigned long currentMillis = millis();

  // Print encoder count every second
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Print the current encoder count
    Serial.print("Encoder Count: ");
    Serial.println(encoderCount);

    // Reset encoder count
    encoderCount = 0;
  }
}
