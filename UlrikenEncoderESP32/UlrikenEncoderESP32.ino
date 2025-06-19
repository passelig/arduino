#include <WiFi.h>
#include <WebServer.h>

// === Rotary Encoder ===
#define ENCODER_CLK 2
#define ENCODER_DT  4

int lastClkState;
volatile int counter = 0;

// === Wi-Fi AP credentials ===
const char* ssid = "Ulriken";
const char* password = "Hemmelig";

// === Create web server on port 80 ===
WebServer server(80);

// === Handle GET /counter ===
void handleCounter() {
  String json = "{\"counter\": " + String(counter) + "}";
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);

  // Rotary encoder pin setup
  pinMode(ENCODER_CLK, INPUT_PULLUP);
  pinMode(ENCODER_DT, INPUT_PULLUP);
  lastClkState = digitalRead(ENCODER_CLK);

  // Set up Wi-Fi AP
  WiFi.softAP(ssid, password);
  Serial.println("Access Point started");
  Serial.println("IP address: " + WiFi.softAPIP().toString());

  // REST API route
  server.on("/counter", HTTP_GET, handleCounter);
  server.begin();
}

void loop() {
  // Rotary encoder logic
  int clkState = digitalRead(ENCODER_CLK);
  int dtState = digitalRead(ENCODER_DT);

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

  // Handle incoming HTTP requests
  server.handleClient();

  delay(1);
}
