#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

static const char my_ca_crt[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDpzCCAo+gAwIBAgIUNwXclLpxcnUQWwFKXkWtJRTLFq4wDQYJKoZIhvcNAQEL
BQAwYzELMAkGA1UEBhMCSW4xCzAJBgNVBAgMAlRuMRAwDgYDVQQHDAdDaGVubmFp
MRUwEwYDVQQKDAxGZW9zIFB2dCBMdGQxHjAcBgNVBAsMFUNlcnRpZmljYXRlIEF1
dGhvcml0eTAeFw0yNDAzMDExMzU2NDVaFw0yNTAzMDExMzU2NDVaMGMxCzAJBgNV
BAYTAkluMQswCQYDVQQIDAJUbjEQMA4GA1UEBwwHQ2hlbm5haTEVMBMGA1UECgwM
RmVvcyBQdnQgTHRkMR4wHAYDVQQLDBVDZXJ0aWZpY2F0ZSBBdXRob3JpdHkwggEi
MA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCLgKOl1H/kZVC+sJ8V2MLkEogo
NR/f8Rp8vuALEg50Xq68vC5L0hLJket3uSEEfWGz6vl+DSWB0z23PdrBejdjNAei
jkQDC2vG+dDknlCgs1CHivRP9AaFoL56mEFjq9dvLrbnee0IEWyYsZTCC+B281Vt
dgHLU7fqtL9XH5IyaC8Z9sNkJwUVHyvJgWtjdwyzkl4TiZIwjfKMuJWb4zmPHVKb
yxHyY9CzDCvirqep3GjxZQKoQ4y6zBU9Mua85BsllDkl1PWZvri7PS3q31CTtL5X
QxKRJ4ULc3tFTgt3kcM7wNB5FMZQUTG2VIRNTwPvl4oMPJULE/cHk69Rf12BAgMB
AAGjUzBRMB0GA1UdDgQWBBSUrWLMW75Xvq4AtgPPaSqsCZLOODAfBgNVHSMEGDAW
gBSUrWLMW75Xvq4AtgPPaSqsCZLOODAPBgNVHRMBAf8EBTADAQH/MA0GCSqGSIb3
DQEBCwUAA4IBAQAr1dK6BPZf3aFs1GcGUgzBB868dwbLZXhQpfc2yB907J6D4XCO
y1qZ+UQjw00gKyxljNB7apHPXKs5O06pprGC1m2Z+aTuWG53HvRDjugXCACYYibs
BGF6NL9r8PT6QiduRDpnn4eeM3BpjcGAMz1n0Mc4LkWhuIoEQrmHPAVi2N1/X8Hl
fYKRv1+hTShMHMyUj/xZJGRIQhGSHPTcwPPN9PjU7L0dJzwzRvDw4tJb0GKY5XDK
L4qXqSQIioXwsQY9DGw46aqE9orOTAI6FbJgF8lT6rkf/kHOHb/2+P1u/hlSoxep
nhGXvIt1q6gnXFNHuUiwnlCE4Eh4e504Mvku
-----END CERTIFICATE-----
)EOF";

const char* ssid = "ssid";
const char* password = "ssid password";
const char* mqtt_server = "mqtt user name";
const uint16_t mqtt_server_port = port value;
const char* mqttTopic = "topic";
const char* mqtt_username = "mqtt username";
const char* mqtt_password = "mqtt password";

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
    if (mqttClient.connect(mqttClientId.c_str(), mqtt_username, mqtt_password)) {
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
  mqttClient.publish(mqttTopic,("tester : " + myCurrentTime).c_str());
  mqttClient.loop();
  timeClient.update();
}
