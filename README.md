# Water Pump Controller Using ESP-32
This water pump controller has two implementations using Arduino IDE. One is with Blynk App and the other with a web server. I've made use of the ultrasonic sensor to measure the water level in the tank. Accordingly, the motor turns on/off through a relay.

## Materails Required
* ESP-32 
* Water Submursive Pump
* Ultrasonic Sensor HC-SR04
* Bread Board Module
* 5V Single Channel Relay
* Breadboard
* Male to Male Wires
* Male to Female Wires

## Libraries Used
#### For implementation with Web Server
* WiFi.h
* ESPAsyncWebServer.h
* Ultrasonic.h
* SPIFFS.h
* Wire.h
* string.h
* Preferences.h

#### For implementation using Blynk
* WiFi.h
* WiFiClient.h
* BlynkSimpleEsp32.h




