/*
    Part 1 of Seminar:
    Get data from openweather and send to thingSpeak
*/


#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

#include "ThingSpeak.h"
#include "weatherData.h"

const char* ssid = "Wokwi-GUEST";
const char* password = "";

unsigned long myChannelNumber = 2229655;
const char* myWriteAPIKey = "VORDQY6MK8IHS74E";

void wifiConnect() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected");
}

WiFiClient client;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Connecting to WiFi");

  wifiConnect();

  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  ThingSpeak.begin(client);
}

void loop() {
  WiFiClient client;
  HTTPClient http;

  delay(500);
  http.begin(client, "http://api.openweathermap.org/data/2.5/weather?lat=10.734174&lon=106.6843834&units=metric&appid=9b78bba84df8146fea65001986fa0aa9");
  http.GET();

  DynamicJsonDocument doc(1024);
  DeserializationError err = deserializeJson(doc, http.getStream());
  if (err) {
  Serial.print(F("deserializeJson() failed with code "));
  Serial.println(err.f_str());
  }

  weatherData data;

  data.inputData(doc);
  
  ThingSpeak.setField(1, data.temp);
  ThingSpeak.setField(2, data.temp_min);
  ThingSpeak.setField(3, data.temp_max);
  ThingSpeak.setField(4, data.pressure);
  ThingSpeak.setField(5, data.humidity);
  ThingSpeak.setField(6, data.visibility);
  ThingSpeak.setField(7, data.speed);
  ThingSpeak.setField(8, data.all);
  int returncode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  // Check return code
  if (returncode == 200) {
    Serial.println("Channel update successfull.");
  }
  else {
    Serial.println("Problem updating channel. HTTP error code ");
  }
  // end
  http.end();

  Serial.print("Completed!");
  delay(1800000); // Update data every 30 min
}
