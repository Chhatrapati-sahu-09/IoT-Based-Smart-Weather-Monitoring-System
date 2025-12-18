//HP14S 5420
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// LCD Setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// DHT11 Setup
#define DHT_PIN 4
#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);

// WiFi Credentials
const char* ssid = "HP14S 5420";
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// OpenWeatherMap API - TEST WITH SIMPLE CITY
const String apiKey = "YOUR_API_KEY";
const String city = "Durg"; // Simple city name
const String city = "YOUR_CITY";
const String countryCode = "IN";
String weatherAPI = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + apiKey + "&units=metric";

// Variables
float roomTemp, roomHumidity;
float outsideTemp = 0;
float outsideHumidity = 0;
String weatherCondition = "No Data";
bool wifiConnected = false;
unsigned long lastUpdate = 0;
const long updateInterval = 30000;

void setup() {
  Serial.begin(115200);
  Serial.println("Weather Station Starting...");
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Starting...");
  
  // Initialize DHT
  dht.begin();
  delay(2000);
  
  // Try WiFi connection
  tryWiFiConnection();
}

void loop() {
  // Read DHT sensor data
  readDHTData();
  
  // Try to get weather data if WiFi connected
  if (wifiConnected && (millis() - lastUpdate >= updateInterval)) {
    lastUpdate = millis();
    getWeatherData();
  }
  
  // Display rotating screens
  displayRotatingScreens();
  delay(5000); // Change screen every 5 seconds
}

void tryWiFiConnection() {
  Serial.println("Attempting WiFi Connection...");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connecting");
  
  WiFi.begin(ssid, password);
  
  unsigned long startTime = millis();
  const unsigned long timeout = 30000; // 30 seconds
  
  while (WiFi.status() != WL_CONNECTED && (millis() - startTime) < timeout) {
    delay(500);
    Serial.print(".");
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;
    Serial.println("\nWiFi CONNECTED!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi Connected");
    lcd.setCursor(0, 1);
    lcd.print("IP: ");
    lcd.print(WiFi.localIP().toString().substring(0, 13));
    delay(3000);
  } else {
    wifiConnected = false;
    Serial.println("\nWiFi FAILED!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi Failed");
    lcd.setCursor(0, 1);
    lcd.print("Local Mode Only");
    delay(3000);
  }
}

void readDHTData() {
  float newTemp = dht.readTemperature();
  float newHumidity = dht.readHumidity();
  
  if (!isnan(newTemp) && !isnan(newHumidity)) {
    roomTemp = newTemp;
    roomHumidity = newHumidity;
    Serial.print("DHT11 - Temp: ");
    Serial.print(roomTemp);
    Serial.print("C, Humidity: ");
    Serial.print(roomHumidity);
    Serial.println("%");
  } else {
    Serial.println("DHT11 Read Failed!");
  }
}

void getWeatherData() {
  Serial.println("=== ATTEMPTING TO GET WEATHER DATA ===");
  Serial.print("API URL: ");
  Serial.println(weatherAPI);
  
  if (!wifiConnected || WiFi.status() != WL_CONNECTED) {
    Serial.println("ERROR: WiFi not connected");
    wifiConnected = false;
    return;
  }
  
  HTTPClient http;
  http.begin(weatherAPI);
  http.setTimeout(10000);
  
  Serial.println("Sending HTTP GET request...");
  int httpCode = http.GET();
  Serial.print("HTTP Response Code: ");
  Serial.println(httpCode);
  
  if (httpCode == 200) {
    String payload = http.getString();
    Serial.print("API Response: ");
    Serial.println(payload);
    
    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, payload);
    
    if (!error) {
      outsideTemp = doc["main"]["temp"];
      outsideHumidity = doc["main"]["humidity"];
      weatherCondition = doc["weather"][0]["main"].as<String>();
      
      Serial.println("=== WEATHER DATA PARSED ===");
      Serial.print("Outside Temp: ");
      Serial.println(outsideTemp);
      Serial.print("Outside Humidity: ");
      Serial.println(outsideHumidity);
      Serial.print("Condition: ");
      Serial.println(weatherCondition);
      Serial.println("===========================");
      
    } else {
      Serial.println("ERROR: JSON Parsing Failed");
      Serial.println(error.c_str());
      weatherCondition = "JSON Error";
    }
  } else if (httpCode == 401) {
    Serial.println("ERROR: Invalid API Key");
    weatherCondition = "API Key Invalid";
  } else if (httpCode == 404) {
    Serial.println("ERROR: City not found");
    weatherCondition = "City Not Found";
  } else {
    Serial.println("ERROR: HTTP Request Failed");
    weatherCondition = "HTTP Error " + String(httpCode);
  }
  
  http.end();
  Serial.println("=== WEATHER UPDATE COMPLETE ===");
}

void displayRotatingScreens() {
  static int screenNumber = 0;
  screenNumber = (screenNumber + 1) % 3;
  
  lcd.clear();
  
  switch(screenNumber) {
    case 0:
      // Room Data
      lcd.setCursor(0, 0);
      lcd.print("Room:");
      lcd.setCursor(0, 1);
      lcd.print(roomTemp, 1);
      lcd.print("C ");
      lcd.print(roomHumidity, 0);
      lcd.print("%");
      break;
      
    case 1:
      // Outside Data
      lcd.setCursor(0, 0);
      lcd.print("Outside:");
      lcd.setCursor(0, 1);
      if (outsideTemp > 0) {
        lcd.print(outsideTemp, 1);
        lcd.print("C ");
        lcd.print(weatherCondition);
      } else {
        lcd.print("No Data");
      }
      break;
      
    case 2:
      // Status
      lcd.setCursor(0, 0);
      lcd.print("Status:");
      lcd.setCursor(0, 1);
      if (wifiConnected) {
        lcd.print("Online");
      } else {
        lcd.print("Offline");
      }
      break;
  }
}
