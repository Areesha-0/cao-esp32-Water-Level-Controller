#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include<Ultrasonic.h>
#include "SPIFFS.h"
#include <Wire.h>

//network credentials
const char* ssid = "muhammadasif7";
const char* password = "asif1972";

const int trigPin = 33;
const int echoPin = 32;

Ultrasonic ultrasonic_sensor(trigPin, echoPin);

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String getDistance(){
  //read distance
  float distance = ultrasonic_sensor.read();

  if(isnan(distance)){
     Serial.println("Did not get any reading from the ultrasonic sensor");
     return "";   
  }
  else{
     Serial.println("distance: ");
     Serial.println(distance);
     return String(distance);
  }
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

    // Initialize SPIFFS
  if (! SPIFFS.begin ()) {
    Serial.println ("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });

  server.on ("/distance", HTTP_GET, [] (AsyncWebServerRequest * request) {
    request-> send_P (200, "text / plain", getDistance(). c_str ());
  });
  // Start server
  server.begin();
}
  
void loop() {
}
