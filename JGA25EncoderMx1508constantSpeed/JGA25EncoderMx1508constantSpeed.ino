// Pin definitions
const int encoderPinA = 2;    // Encoder pin A (interrupt pin)
const int encoderPinB = 3;    // Encoder pin B
const int motorPinA = 5;      // MX1508 IN1
const int motorPinB = 6;     // MX1508 IN2
const int desiredEncoderCount = 150;  // Desired Power in ticks per 100ms

volatile long encoderCount = 0;   // Encoder count

// Variables for time tracking
unsigned long previousMillis = 0;
unsigned long previousControlMillis = 0;
const long interval = 1000;       // Interval for printing (1 second)
const long controlInterval = 100; // Control interval (100ms)

// PWM control
int motorPower = 128;             // Initial motor Power (PWM value 0-255)
float Kp = 0.8;                   // Proportional gain for the P-controller

// Interrupt service routine (ISR) for encoder
void encoderISR() {
    encoderCount++;
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

  // Print the labels for the Serial Plotter
  // The '#' indicates that the following is a label
  Serial.println("#Error,EncoderCount,MotorPower");
  
  // Set initial motor Power (PWM)
  analogWrite(motorPinA, motorPower);
  analogWrite(motorPinB, 0); // Set one pin low to run the motor in one direction
}

void loop() {
  unsigned long currentMillis = millis();

  // Motor Power control every 100ms
  if (currentMillis - previousControlMillis >= controlInterval) {
    previousControlMillis = currentMillis;

    // Calculate the error between desired and actual Power
    long error = desiredEncoderCount - encoderCount;

    // Adjust motor Power using a proportional controller
    motorPower += Kp * error;

    // Constrain motor Power to valid PWM range (0-255)
    motorPower = constrain(motorPower, 0, 255);

    // Apply the new motor Power (PWM)
    analogWrite(motorPinA, motorPower);
    analogWrite(motorPinB, 0); // Keep running motor in the same direction

    // Print values for Serial Plotter in CSV format
    Serial.print("Avvik");
    Serial.print(":");
    Serial.print(error);
    Serial.print(" ");
    Serial.print("encoderTeller");
    Serial.print(":");
    Serial.print(encoderCount);
    Serial.print(" ");
    Serial.print("motorkraft(PWM)");
    Serial.print(":");
    Serial.println(motorPower);

    // Reset encoder count for the next 100ms
    encoderCount = 0;
  }
}
