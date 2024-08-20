 #include <WiFi.h>

// Replace with your network credentials
const char* ssid = "Gunnar";
const char* password = "Hemmelig";

// Set GPIO 2 (Onboard LED) as output
const int ledPin = 2;

// Create an instance of the WiFiServer class
WiFiServer server(80);

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Set the LED pin as an output
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Turn LED off by default

  // Connect to Wi-Fi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
}

void loop() {
  // Listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<h1>ESP32 LED Control</h1>");
            client.println("<p><a href=\"/LEDON\"><button>Turn On</button></a></p>");
            client.println("<p><a href=\"/LEDOFF\"><button>Turn Off</button></a></p>");
            client.println("</html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else {
            // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {
          // you got a character on the current line
          currentLine += c;
        }

        // Check to see if the client request was "GET /LEDON" or "GET /LEDOFF":
        if (currentLine.endsWith("GET /LEDON")) {
          digitalWrite(ledPin, HIGH); // Turn LED on
        }
        if (currentLine.endsWith("GET /LEDOFF")) {
          digitalWrite(ledPin, LOW); // Turn LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
