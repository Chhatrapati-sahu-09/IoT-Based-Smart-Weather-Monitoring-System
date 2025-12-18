# IoT-Based Smart Weather Monitoring System

[![Arduino](https://img.shields.io/badge/platform-Arduino-blue.svg)](https://www.arduino.cc/) [![ESP32](https://img.shields.io/badge/board-ESP32-green.svg)](https://www.espressif.com/en/products/socs/esp32) [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## Introduction

The rapid advancement of the Internet of Things (IoT) has enabled real-time monitoring and automation of environmental parameters. Weather monitoring is one of the most widely adopted IoT applications due to its importance in daily decision-making, comfort management, and environmental awareness.

This project integrates on-device sensor data with cloud-based weather services to provide a dual-perspective analysis of environmental conditions. Indoor temperature and humidity are measured using a DHT11 sensor, while real-time outdoor weather data is fetched from the OpenWeatherMap API using WiFi connectivity through an ESP32 microcontroller. All collected data is displayed on a 16×2 I2C LCD, which rotates automatically between indoor weather, outdoor weather, and network/connectivity status.

The system offers a compact, real-time, and user-friendly solution while demonstrating core IoT concepts such as sensor integration, microcontroller programming, API communication, and real-time data visualization.

## Requirement Analysis and System Specification

### Hardware Requirements
- ESP32 Development Board
- DHT11 Temperature and Humidity Sensor
- 16×2 I2C LCD Display
- Breadboard and Jumper Wires
- USB Cable and 5V Power Supply

### Software Requirements
- Arduino IDE
- ESP32 Board Package
- Required Libraries:
  - WiFi.h
  - HTTPClient.h
  - ArduinoJson.h
  - LiquidCrystal_I2C.h
  - DHT.h
- OpenWeatherMap API Key
- Serial Monitor for debugging

### Functional Specifications
- Measure indoor temperature and humidity using DHT11 sensor
- Fetch outdoor temperature, humidity, and weather conditions via OpenWeatherMap API
- Display information on a 16×2 I2C LCD with automatic screen rotation
- Indicate WiFi connectivity and network status
- Provide continuous and real-time monitoring

### Non-Functional Specifications
- System must operate continuously (24/7 stability)
- Low power consumption using ESP32
- Accurate and real-time data acquisition
- Simple user interface and easy maintenance

## System Design

### System Architecture
- **Sensing Layer:** DHT11 sensor captures indoor temperature and humidity
- **Processing Layer:** ESP32 processes sensor data and API responses
- **Communication Layer:** ESP32 connects to WiFi and communicates with OpenWeatherMap API
- **Application Layer:** 16×2 I2C LCD displays indoor weather, outdoor weather, and network status

### Data Flow
1. DHT11 sends temperature and humidity data to ESP32
2. ESP32 sends an HTTP GET request to OpenWeatherMap API
3. API returns weather data in JSON format
4. ESP32 parses required values using ArduinoJson library
5. LCD displays data in rotating sequence: Indoor Weather → Outdoor Weather → Network Status

### System Diagram

```
DHT11 → ESP32 → WiFi → OpenWeatherMap API → ESP32 → LCD Display
```

## Implementation, Testing, and Maintenance

### Implementation
- ESP32 programmed using Arduino IDE
- WiFi credentials and API key configured within the source code
- DHT11 connected to a digital GPIO pin
- LCD interfaced using I2C protocol
- JSON data parsed using ArduinoJson library
- Rotating LCD display implemented using time-based switching logic

### Testing
- Sensor readings validated by comparison with standard thermometer and hygrometer
- API responses verified through Serial Monitor for correct JSON parsing
- WiFi connectivity tested under varying signal strengths
- LCD display tested for readability and smooth screen transitions

### Maintenance
- Replace DHT11 sensor if measurement accuracy degrades
- Update API key when expired
- Modify WiFi credentials if network changes
- Modular code structure supports easy debugging and future upgrades

## Results and Discussion

The developed system successfully measures indoor temperature and humidity while simultaneously retrieving real-time outdoor weather data from the OpenWeatherMap API. The rotating LCD display provides clear and organized visualization of environmental conditions.

By comparing indoor and outdoor data, users can assess heat retention, ventilation needs, and comfort levels. The integration of cloud-based weather data demonstrates efficient IoT-cloud communication, while the ESP32 ensures reliable performance and continuous operation.

## Conclusion

The IoT-Based Smart Weather Monitoring System effectively combines sensor data and cloud services to deliver accurate, real-time environmental insights, making it a practical and scalable IoT solution.
