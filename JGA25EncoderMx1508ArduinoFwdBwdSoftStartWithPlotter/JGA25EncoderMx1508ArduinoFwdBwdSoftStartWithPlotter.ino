// Define control pins
const int IN1 = 9; // Control pin 1
const int IN2 = 10; // Control pin 2

// Define PWM limits
const int minSpeed = 50;  // Minimum speed value
const int maxSpeed = 255; // Maximum speed value

// Define time for acceleration and deceleration
const int accelTime = 1000; // 1 second for acceleration
const int runTime = 1000;   // 1 second at full speed
const int stopTime = 1000;  // 1 second stop time

// Timing variables
unsigned long previousMillis = 0;
unsigned long previousPrintMillis = 0;
const unsigned long printInterval = 100; // Print every 100 milliseconds
int currentSpeed = minSpeed;
int speedStep = 1; // Speed increment per loop

// State machine variables
enum State { ACCELERATE_FORWARD, RUN_FORWARD, DECELERATE_FORWARD, STOP_AFTER_FORWARD, STOP_AFTER_BACKWARD, ACCELERATE_BACKWARD, RUN_BACKWARD, DECELERATE_BACKWARD };
State currentState = ACCELERATE_FORWARD;

void setup() {
  // Set control pins as outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  
  // Start serial communication
  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();

  // Print speed every 100 ms
  if (currentMillis - previousPrintMillis >= printInterval) {
    previousPrintMillis = currentMillis;
    if (currentState == ACCELERATE_FORWARD || currentState == DECELERATE_FORWARD || currentState == RUN_FORWARD) {
      Serial.println(currentSpeed); // Forward speeds
    } else if (currentState == ACCELERATE_BACKWARD || currentState == DECELERATE_BACKWARD || currentState == RUN_BACKWARD) {
      Serial.println(-currentSpeed); // Backward speeds
    } else {
      Serial.println(0); // Stop state
    }
  }

  switch (currentState) {
    case ACCELERATE_FORWARD:
      if (currentMillis - previousMillis >= accelTime / (maxSpeed - minSpeed)) {
        previousMillis = currentMillis;
        currentSpeed += speedStep;
        analogWrite(IN1, currentSpeed);
        analogWrite(IN2, 0);

        if (currentSpeed >= maxSpeed) {
          currentState = RUN_FORWARD;
          previousMillis = currentMillis;
        }
      }
      break;

    case RUN_FORWARD:
      if (currentMillis - previousMillis >= runTime) {
        currentState = DECELERATE_FORWARD;
        previousMillis = currentMillis;
      }
      break;

    case DECELERATE_FORWARD:
      if (currentMillis - previousMillis >= accelTime / (maxSpeed - minSpeed)) {
        previousMillis = currentMillis;
        currentSpeed -= speedStep;
        analogWrite(IN1, currentSpeed);
        analogWrite(IN2, 0);

        if (currentSpeed <= minSpeed) {
          currentState = STOP_AFTER_FORWARD;
          previousMillis = currentMillis;
        }
      }
      break;

    case STOP_AFTER_FORWARD:
      analogWrite(IN1, 0);
      analogWrite(IN2, 0);
      if (currentMillis - previousMillis >= stopTime) {
        currentState = ACCELERATE_BACKWARD;
        previousMillis = currentMillis;
        currentSpeed = minSpeed;
      }
      break;

    case STOP_AFTER_BACKWARD:
      analogWrite(IN1, 0);
      analogWrite(IN2, 0);
      if (currentMillis - previousMillis >= stopTime) {
        currentState = ACCELERATE_FORWARD;
        previousMillis = currentMillis;
        currentSpeed = minSpeed;
      }
      break;

    case ACCELERATE_BACKWARD:
      if (currentMillis - previousMillis >= accelTime / (maxSpeed - minSpeed)) {
        previousMillis = currentMillis;
        currentSpeed += speedStep;
        analogWrite(IN1, 0);
        analogWrite(IN2, currentSpeed);

        if (currentSpeed >= maxSpeed) {
          currentState = RUN_BACKWARD;
          previousMillis = currentMillis;
        }
      }
      break;

    case RUN_BACKWARD:
      if (currentMillis - previousMillis >= runTime) {
        currentState = DECELERATE_BACKWARD;
        previousMillis = currentMillis;
      }
      break;

    case DECELERATE_BACKWARD:
      if (currentMillis - previousMillis >= accelTime / (maxSpeed - minSpeed)) {
        previousMillis = currentMillis;
        currentSpeed -= speedStep;
        analogWrite(IN1, 0);
        analogWrite(IN2, currentSpeed);

        if (currentSpeed <= minSpeed) {
          currentState = STOP_AFTER_BACKWARD;
          previousMillis = currentMillis;
        }
      }
      break;
  }
}
