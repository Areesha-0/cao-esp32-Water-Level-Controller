#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include<Ultrasonic.h>
#include "SPIFFS.h"
#include <Wire.h>

//network credentials
const char* ssid = "muhammadasif7";
const char* password = "asif1972";

int relayGPIO = 13;
const int trigPin = 33;
const int echoPin = 32;
const char* PARAM_INPUT_2 = "state";

Ultrasonic ultrasonic_sensor(trigPin, echoPin);

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String relayState(){
  if(digitalRead(relayGPIO)){
    return "checked";
  }
  else{
    return "";
  }
  return "";
}

String processor(const String& var){
  //Serial.println(var);
  if(var == "BUTTONPLACEHOLDER"){
    String button ="";
    String relayStateValue = relayState();
    button = "<h4> - GPIO " + String(relayGPIO)+ "</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox()\" "+ relayStateValue +"><span class=\"slider\"></span></label>";
    return button;
  }
  return String();
}

String get_waterlevel(){
  //read distance
  float water_level = ultrasonic_sensor.read();

  if(isnan(water_level)){
     Serial.println("Did not get any reading from the ultrasonic sensor");
     return "";   
  }
  else{
     Serial.println("water level (cm): ");
     Serial.println(water_level);
     return String(water_level);
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

  //relay is in NC configuration
  pinMode(relayGPIO, OUTPUT);
  digitalWrite(relayGPIO, LOW);

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
    request-> send_P(200, "text / plain", get_waterlevel(). c_str ());
  });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", processor("BUTTONPLACEHOLDER").c_str());
  });

  // Send a GET request to <ESP_IP>/update?state=<inputMessage2>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String relay_state;
    String inputParam2;
    if (request->hasParam(PARAM_INPUT_2)) {
      relay_state = request->getParam(PARAM_INPUT_2)->value();
      inputParam2 = PARAM_INPUT_2;
      digitalWrite(relayGPIO, relay_state.toInt());
    }

    Serial.println("Motor's state (0->off/1->on): ");
    Serial.println(relay_state);
    request->send_P(200, "text/plain", "OK");
  });
  // Start server
  server.begin();
}
  
void loop() {
}
