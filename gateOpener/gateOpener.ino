// Define pins
const int IN1 = 6;
const int IN2 = 7;
const int ENA = 8;
const int buttonInside = 2;
const int sensorInside = 3;
int buttonPressed = false;
int sensorTriggered = false;
unsigned long openTime = 0;
bool doorIsOpen = false;  // Keeps track of the door state

void setup() {
  // Initialize the motor control pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  // Initialize the switch pin with internal pull-up
  pinMode(buttonInside, INPUT_PULLUP);

  // Ensure the motor is off initially
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);

  Serial.begin(9600);  // Initialize serial communication for debugging
}

void loop() {
  // Read the switch state
  if (digitalRead(buttonInside) == 0) {
    Serial.println("ButtonPressed");
    buttonPressed = true;
  } else {
    buttonPressed = false;
  }

  if (digitalRead(sensorInside) ==1 ) {
    Serial.println("sensorTriggered");
    sensorTriggered = true;
  } else {
    sensorTriggered = false;
  }

  // Toggle door state on switch press
  if (buttonPressed||sensorTriggered) {  // Switch pressed
    if (doorIsOpen) {
      closeDoor();
    } else {
      openTime = millis();  // Start Record the time when the door is opened
      openDoor();
    }
  }
  if (doorIsOpen && (millis() - openTime >= 5000)) {
    Serial.println("Open timeout elapsed, Closing");
    closeDoor();
  }
  delay(100);  // Loop delay
}

void openDoor() {
  Serial.println("Opening door...");
  doorIsOpen = true;
  // Accelerate the motor to open the door
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  for (int speed = 0; speed <= 255; speed += 5) {
    analogWrite(ENA, speed);
    delay(50);  // Acceleration delay
  }
  delay(3000);
}

void closeDoor() {
  Serial.println("Closing door...");
  doorIsOpen = false;
  // Accelerate the motor to close the door
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  for (int speed = 0; speed <= 255; speed += 5) {
    analogWrite(ENA, speed);
    delay(5);  // Acceleration delay
  }
  delay(3000);
}
