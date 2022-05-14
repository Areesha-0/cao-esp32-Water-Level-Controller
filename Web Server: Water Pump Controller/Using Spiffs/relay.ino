#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include<Ultrasonic.h>
#include "SPIFFS.h"
#include <Wire.h>

//network credentials
const char* ssid = "muhammadasif7";
const char* password = "asif1972";

const int relay_pin = 13;
// Stores relay state
String relay_state;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Replaces placeholder with relay's state value
String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if(digitalRead(relay_pin)){
      relay_state = "ON";
    }
    else{
      relay_state = "OFF";
    }
    Serial.print(relay_state);
    return relay_state;
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  //relay is in NC configuration
  pinMode(relay_pin, OUTPUT);
  digitalWrite(relay_pin, LOW);

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
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to set relay to HIGH
  server.on("/motor_on", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(relay_pin, HIGH);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to set GPIO to LOW
  server.on("/motor_off", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(relay_pin, LOW);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  // Start server
  server.begin();
}
  
void loop() {
}
