/*
  ESP32 HTTPClient Jokes API Example

  https://wokwi.com/projects/342032431249883731

  Copyright (C) 2022, Uri Shaked
*/

#include <WiFi.h>
#include <NTPClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include "ThingSpeak.h"

const char* ssid = "Wokwi-GUEST";
const char* password = "";

unsigned long myChannelNumber = 2229655;
const char* myReadAPIKey = "Z95ZQ0A36D8VEHV5";

#define TFT_DC 2
#define TFT_CS 15
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

void wifiConnect() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected");
}

WiFiUDP ntpUDP;
WiFiClient client;
NTPClient timeClient(ntpUDP);

void setup() {

  wifiConnect();

  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  ThingSpeak.begin(client);
  timeClient.begin();
  
  tft.begin();
  tft.setRotation(1);

  tft.setTextSize(2);
  tft.setTextColor(ILI9341_WHITE);
}

void loop() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  timeClient.update();
  String time = timeClient.getFormattedTime();
  tft.setCursor(20, 30);
  tft.println(time);

  int t = ThingSpeak.readIntField(myChannelNumber, 1, myReadAPIKey);
  int h = ThingSpeak.readIntField(myChannelNumber, 5, myReadAPIKey);

  tft.setCursor(20, 120);
  tft.println("Temperature: ");

  tft.setCursor(250, 120);
  tft.print(t);

  tft.setCursor(20, 160);
  tft.println("Humidity: ");

  tft.setCursor(250, 160);
  tft.print(h);

  delay(60000);
}