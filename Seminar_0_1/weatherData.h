#pragma once
#include <ArduinoJson.h>

struct weatherData {
  // cord
  float Lon; // Longitude of the location
  float Lat; // Latitude of the location

  // weather
  int w_id; // Weather condition id
  String main; // Group of weather parameters
  String description;  // Weather condition within the group
  String icon; // Weather icon

  // base
  String base; // Internal parameter

  // main
  float temp; // Temperature. Unit: Celsius
  float feels_like; // Temperature. This temperature parameter accounts for the human perception of weather.
  float temp_min; // Minimum temperature at the moment. This is minimal currently observed temperature
  float temp_max; // Maximum temperature at the moment. This is maximal currently observed temperature
  float pressure; // Atmospheric pressure. Unit: hPa
  int humidity; // Humidity, %

  // Visibility
  int visibility; // Visibility, meter. The maximum value of the visibility is 10km

  // wind
  float speed; // Wind speed. Unit: meter/sec
  int deg; // Wind direction, degrees (meteorological)

  // clouds
  int all; // cloudiness, %

  /*
  // rain, if available
  int 1h; // Rain volume for the last 1 hour, mm
  int 3h; // Rain volume for the last 3 hour, mm
  */

  //sys
  int type; // Internal parameter
  int s_id; // Internal parameter
  String country; // Country code 
  int sunrise; // Sunrise time, unix, UTC
  int sunset; // Sunset time, unix, UTC

  //timezone
  int timezone; // Shift in seconds from UTC

  //city id
  int c_id; 

  // city name
  String name;

  void inputData(DynamicJsonDocument doc);
  void outputData();
  
};

void weatherData::inputData(DynamicJsonDocument doc) {
  this->Lon = doc["lon"].as<float>();
  this->Lat = doc["lat"].as<float>();

  this->w_id = doc["weather"][0]["id"].as<int>();
  this->main = doc["weather"][0]["main"].as<String>();
  this->description = doc["weather"][0]["description"].as<String>(); 
  this->icon = doc["weather"][0]["icon"].as<int>();

  this->base = doc["base"].as<String>();

  this->temp = doc["main"]["temp"].as<float>();
  this->feels_like = doc["main"]["feels_like"].as<float>();
  this->temp_min = doc["main"]["temp_min"].as<float>();
  this->temp_max = doc["main"]["temp_max"].as<float>();
  this->pressure = doc["main"]["pressure"].as<float>();
  this->humidity = doc["main"]["humidity"].as<int>();

  this->visibility = doc["visibility"].as<int>();

  this->speed = doc["wind"]["speed"].as<float>();
  this->deg = doc["wind"]["deg"].as<int>();

  this->all = doc["clouds"]["all"].as<int>();

  this->type = doc["sys"]["type"].as<int>();
  this->s_id = doc["sys"]["id"].as<int>();
  this->country = doc["sys"]["country"].as<String>();
  this->sunrise = doc["sys"]["sunrise"].as<int>();
  this->sunset = doc["sys"]["sunset"].as<int>();

  this->timezone = doc["timezone"].as<int>();
  this->c_id = doc["id"].as<int>();
  this->name = doc["name"].as<String>();
}