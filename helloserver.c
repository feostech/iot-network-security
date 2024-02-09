#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "ssid";
const char* password = "ssid password";

ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/plain", " Hello From ESP 8266 ! ");
}
void setup() {
  Serial.begin(115200);  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");

while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.println("Connecting to WiFi...");
  }
    Serial.println("Connected to WiFi");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  // Start the server
    server.on("/", handleRoot);
    server.begin();
    Serial.println("HTTP server started");
}
void loop() {
    server.handleClient();
}
