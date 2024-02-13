
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

const char* ssid = "ssid";
const char* password = "ssid-password";
const char* mqtt_server = "ip-address";
const uint16_t mqtt_server_port = 1883;
const char* mqttTopic = "topic-name";

#ifdef MQTT_TLS
  WiFiClientSecure wifiClient;
#else
  WiFiClient wifiClient;
#endif
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
    String mqttClientId = "";
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

void setup() {
  Serial.begin(115200);
  setup_wifi();
  mqttClient.setServer(mqtt_server, mqtt_server_port);
}

void loop() {
  if (!mqttClient.connected()) {
    connect();
  }
  String myCurrentTime = timeClient.getFormattedTime();
  mqttClient.publish(mqttTopic,("topic-name : " + myCurrentTime).c_str());
  mqttClient.loop();
  timeClient.update();
}





