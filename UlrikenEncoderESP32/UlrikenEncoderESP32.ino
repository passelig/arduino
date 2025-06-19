// Define pins for the encoder
#define ENCODER_CLK  2   // Connect to CLK pin
#define ENCODER_DT   4   // Connect to DT pin

int lastClkState;
int counter = 0;

void setup() {
  Serial.begin(115200);

  pinMode(ENCODER_CLK, INPUT_PULLUP);
  pinMode(ENCODER_DT, INPUT_PULLUP);

  lastClkState = digitalRead(ENCODER_CLK);
}

void loop() {
  int clkState = digitalRead(ENCODER_CLK);
  int dtState = digitalRead(ENCODER_DT);

  // Detect rotation
  if (clkState != lastClkState) {
    if (dtState != clkState) {
      counter++;
      Serial.println("Rotated CW | Counter: " + String(counter));
    } else {
      counter--;
      Serial.println("Rotated CCW | Counter: " + String(counter));
    }
  }
  lastClkState = clkState;

  delay(1); // Small delay to reduce CPU usage
}
