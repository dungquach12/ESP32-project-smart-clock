#include <WiFi.h>
#include "PubSubClient.h"
#include <ArduinoJson.h>
#include <time.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqttServer = "broker.hivemq.com";
int port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void wifiConnect() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected");
}

void mqttReconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    int id = random(1000);
    char clientID[50];
    sprintf(clientID, "clientID-%d", id);
    if(client.connect(clientID)) {
      Serial.print(clientID);
      Serial.println(" connected");
      client.subscribe("clock/alarm");
    } else {
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print(topic);
  String stMessage;
  for (int i = 0; i < length; i++) {
    stMessage += (char)message[i];
  }
  DynamicJsonDocument doc(512);
  DeserializationError err = deserializeJson(doc, stMessage);
  struct tm lt;
  char buf[100];
  strptime(doc["Time"], "%Y-%m-%dT%H:%M:%S", &lt);
  lt.tm_hour += 7;
}

void setAlarmTime (char* topic, byte* message, unsigned int length) {
  Serial.print(topic);
  String stMessage;
  for (int i = 0; i < length; i++) {
    stMessage += (char)message[i];
  }
  DynamicJsonDocument doc(512);
  DeserializationError err = deserializeJson(doc, stMessage);
  struct tm lt;
  char buf[100];
  strptime(doc["Time"], "%Y-%m-%dT%H:%M:%S", &lt);
  lt.tm_hour += 7;
  strptime(buf, "%H:%M", &lt);
  Serial.println(buf);
  client.publish("clock/setAlarmTime", buf);
}

void setup() {
  Serial.begin(9600);
  Serial.print("Connecting to WiFi");

  wifiConnect();

  client.setServer(mqttServer, port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    mqttReconnect();
  }

  client.loop();

  delay(1000);

  // int temp = random(0, 100);
  // char buffer[50];
  // sprintf(buffer, "%d", temp);
  // Serial.println(buffer);
  // client.publish("weather/temp", buffer);
  // delay(5000);
}
