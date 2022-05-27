# Water Pump Controller Using ESP-32
This water pump controller has two implementations using Arduino IDE. One is with Blynk 2.0 App and the other with a web server. I've made use of the ultrasonic sensor to measure the water level in the tank. Accordingly, the motor turns on/off through a relay.

## Features
#### For implementation with Web Server
This implementation has the following features:
* Motor can be turned on and off 
* Motor turns off automatically when water level reaches 95%
* User can input the percentage to which he wants to fill the tank
* User has to provide the depth of tank before installation but can also edit it later
* Saves tank depth and desired water level permanently and updates it when changed by the user
* Saves and remembers the state of  motor and above mentioned information in case load shedding or wifi unavailability occurs
* Reconnects to wifi automatically
* On the web server user can see a real time graph of water level readings in cm 
* Percentage of tank filled is displayed
* State of the motor is displayed

#### For implementation with Blynk
This implementation has the following features:
* Motor can be turned on and off
* Displays water level in percentage
* Displays water level with aid of a color coded gauge
* Motor automatically turns off when its about to overflow
* Measures tank depth
* Allows the user to select option for filling the tank upto 30%, 50% or 75%
* Displays multiple alerts on web dashboard and provides notifications on mobile phone

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




