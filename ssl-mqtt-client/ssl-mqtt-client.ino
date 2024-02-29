#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

static const char my_ca_crt[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDgzCCAmugAwIBAgIUKV5VePIYPW3l0Mz0y+/7cqOEJ0cwDQYJKoZIhvcNAQEL
BQAwUTELMAkGA1UEBhMCSU4xCzAJBgNVBAgMAlROMRUwEwYDVQQKDAxGRU9TIFBW
VCBMVEQxHjAcBgNVBAsMFUNlcnRpZmljYXRlIEF1dGhvcml0eTAeFw0yNDAyMjcx
MjIxNDNaFw0yNTAyMjYxMjIxNDNaMFExCzAJBgNVBAYTAklOMQswCQYDVQQIDAJU
TjEVMBMGA1UECgwMRkVPUyBQVlQgTFREMR4wHAYDVQQLDBVDZXJ0aWZpY2F0ZSBB
dXRob3JpdHkwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDS1JQNbzrg
v2ozIEu5MrAWp9B5oEUqRr304FRLigO1U6S1JJKnjtC8EKxxdN8wWKmw1biejreV
d8KiYEnZPksNeX4ymTD2r4dI4qfAlmnv30iAtuE50FxXujTN4+wkZLW4bnoCgNzx
Vdbv0RROVxO0n8dOYmm9LcZv/jpcp6q2BNDyMDT0LboCwC7w+9Ugg2XRaj1VXGLC
nISfCAStqJqIltURF9bt5I/8prnPMdSxM1NoudgCCQjHwoZpVLYuTdDjucYLOv0k
vZD+SBTPJW/dtgHQl4RTdKqGzQIw2u400wIL1H+uS+fQPkEoCL+rOpvwhyeQYl6i
P0DPLP6bOUE3AgMBAAGjUzBRMB0GA1UdDgQWBBQ6ADug2Zx1iXgbW23jBqxMoSSr
2TAfBgNVHSMEGDAWgBQ6ADug2Zx1iXgbW23jBqxMoSSr2TAPBgNVHRMBAf8EBTAD
AQH/MA0GCSqGSIb3DQEBCwUAA4IBAQBOIyYXPJwxxxo4JVz711pftL0+WKeoIcQZ
rR2fHfeDqL9c1R8VwjDj6DGQBsRCNu7LeBq1s3hQf6F2ei2NgZviEy2d97P9rvmx
lscrYFrmt6g+YQF8tTt8NsHWPuqtu3THJpjghu6XYx4Dv0Mt3gPOuhTUDyvSWMG/
Rscl5YOiqw0zUX7N713vf7hktGlv7p9Lqxm98QRwCqxxYzZMyCtU4xQVuji3E8Ms
QBEcmlJvBS42CybIpVjFE+23h7qukRj96KYUH9Xw/EWOH5kXMT+TEx2B7mLX1Wz+
TZYXgZr4Wk7VIz7yHOW8uGvm4PPa3+GnPWp6r+S5OkrAnFcKaG7g
-----END CERTIFICATE-----
)EOF";

const char* ssid = "ssid";
const char* password = "ssid-password";
const char* mqtt_server = "mqtt-ip-address";
const uint16_t mqtt_server_port = 8883;
const char* mqttTopic = "topic";

BearSSL::WiFiClientSecure wifiClient;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
PubSubClient mqttClient(wifiClient);

void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  timeClient.begin();
  Serial.println("WiFi connected");
}

void connect() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    String mqttClientId = "ESP8266Client-";
    mqttClientId += String(random(0xffff), HEX);
    if (mqttClient.connect(mqttClientId.c_str(), mqtt_server, ssid)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" will try again in 5 seconds");
      delay(5000);
    }
  }
}

void setClock() {
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {  // Wait for the NTP time to be set
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.println("Time is set");  // Print the time

}

void setup() {
  Serial.begin(115200);
  BearSSL::X509List *serverTrustedCA = new BearSSL::X509List(my_ca_crt);
  wifiClient.setTrustAnchors(serverTrustedCA);
  setup_wifi();
  // Configure NTP and get current time
  configTime(0, 0, "pool.ntp.org");  // UTC
  setClock();  // Wait for time synchronization
  mqttClient.setServer(mqtt_server, mqtt_server_port);
}

void loop() {
  if (!mqttClient.connected()) {
    connect();
  }
  String myCurrentTime = timeClient.getFormattedTime();
  mqttClient.publish(mqttTopic,("topic : " + myCurrentTime).c_str());
  mqttClient.loop();
  timeClient.update();
}
