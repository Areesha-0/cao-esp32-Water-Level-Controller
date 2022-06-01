#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include <Ultrasonic.h>
#include "SPIFFS.h"
#include <Wire.h>
#include <string.h>
#include <Preferences.h>

// network credentials
const char *ssid = "iPhone";
const char *password = "hello123";

// params recieved in urls
const char *PARAM_INPUT_1 = "input1"; // getting tank depth
const char *PARAM_INPUT_2 = "input2"; // getting desired water level

// esp32 gpio pins
const int trig_pin = 33;
const int echo_pin = 32;
const int relay_pin = 13;

// tank depth and water level measurements
float tank_depth = 0;
float water_level = 0; // calculated using the ultrasonic sensor
float desired_waterlevel = 90;

bool motor_state;   // saves to nvs
String relay_state; // Stores relay state

Preferences preferences;
Ultrasonic ultrasonic_sensor(trig_pin, echo_pin);

AsyncWebServer server(80); // Create AsyncWebServer object on port 80

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.println("Connected to AP successfully!");
}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.println("Disconnected from WiFi access point");
  Serial.print("WiFi lost connection. Reason: ");
  Serial.println(info.disconnected.reason);
  Serial.println("Trying to Reconnect");
  WiFi.begin(ssid, password);
}

// informs if server not available
void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

// Replaces placeholders in html
String processor(const String &var)
{
  Serial.println(var);
  if (var == "STATE")
  {
    if (digitalRead(relay_pin))
    {
      relay_state = "ON";
    }
    else
    {
      relay_state = "OFF";
    }
    Serial.print(relay_state);
    return relay_state;
  }
  else if (var == "WATERLEVEL")
  {
    return display_waterlevel();
  }
  else if (var == "TANKDEPTH")
  {
    return display_depth();
  }
  else if (var == "PROMPT")
  {
    return display_prompt();
  }
  return String();
}

String get_waterlevel()
{
  //  float previous_waterlevel = water_level;
  float distance_waterToTankTop = ultrasonic_sensor.read();
  // calculating water level from the given sensor reading
  water_level = tank_depth - distance_waterToTankTop;
  water_level = (water_level / tank_depth) * 100;

  Serial.println("tank depth: ");
  Serial.println(tank_depth);

  if (isnan(distance_waterToTankTop))
  {
    Serial.println("Did not get any reading from the ultrasonic sensor");
    return "";
  }
  else
  {
    // turns motor off if water level reaches the requested limit
    if (water_level >= desired_waterlevel)
    {
      digitalWrite(relay_pin, LOW);
      Serial.println("motor turned off at desired water level");
      Serial.println(desired_waterlevel);

      preferences.putBool("state", 0); // saving state in non volatile storage of esp
      Serial.printf("State saved: 0");
    }

    // motor turns off if motor is about to overflow
    if (water_level >= 90)
    {
      digitalWrite(relay_pin, LOW);
      Serial.println("motor was about to overflow");
      Serial.println(water_level);
      preferences.putBool("state", 0);
      Serial.printf("State saved: 0");
    }

    print_details();
    return String(distance_waterToTankTop);
  }
}

String display_waterlevel()
{
  if (isnan(water_level))
  {
    Serial.println("water level not detected!");
    return "";
  }
  else
  {
    return String(water_level);
  }
}

String display_prompt()
{
  if (water_level >= 90)
  {
    return "Water tank is about to overflow.";
  }

  else if (water_level >= desired_waterlevel)
  {
    return "Water tank turned off. Water reached the given water level";
  }

  else if (water_level <= 10)
  {
    return "Water tank is almost empty. Please turn on the motor.";
  }

  else
    return "";
}

String display_depth()
{
  if (isnan(tank_depth))
  {
    Serial.println("tank depth not detected!");
    return "";
  }
  else
  {
    return String(tank_depth);
  }
}

void print_details()
{
  Serial.println("desired water level (cm): ");
  Serial.println(desired_waterlevel);
  Serial.println("current water level percentage: ");
  Serial.println(water_level);
}

void setup()
{

  Serial.begin(115200); // Serial port for debugging purposes

  WiFi.disconnect(true); // delete old config

  delay(1000);

  // wifi connection and reconnection handling
  WiFi.onEvent(WiFiStationConnected, SYSTEM_EVENT_STA_CONNECTED);
  WiFi.onEvent(WiFiGotIP, SYSTEM_EVENT_STA_GOT_IP);
  WiFi.onEvent(WiFiStationDisconnected, SYSTEM_EVENT_STA_DISCONNECTED);

  WiFi.begin(ssid, password);

  Serial.println();
  Serial.println();
  Serial.println("Wait for WiFi... ");

  // creating namespaces in read write mode and reading values
  preferences.begin("tank_depth", false);
  tank_depth = preferences.getFloat("depth", 0); // returns default value 0 if it depth is not mentioned
  preferences.begin("motor_state", false);
  motor_state = preferences.getBool("state", false); // read the last motor state from flash memory

  // relay is in Normally Closed configuration
  pinMode(relay_pin, OUTPUT);
  digitalWrite(relay_pin, motor_state);

  if (!SPIFFS.begin())
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html"); });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/style.css", "text/css"); });

  //for plotting water level on the chart
  server.on("/level", HTTP_GET, [](AsyncWebServerRequest *request){
    float level = tank_depth - get_waterlevel().toFloat();
    String get_level = String("");
    get_level.concat(level);
    request-> send_P(200, "text / plain", get_level.c_str()); }); 

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", String(), false, processor); });

  server.on("/motor_on", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(relay_pin, HIGH); 
    Serial.println(water_level);
    // save motor's state in flash memory
    preferences.putBool("state", 1);
    Serial.printf("State saved: 1");
    
    request->send(SPIFFS, "/index.html", String(), false, processor); });

  // Route to set GPIO to LOW
  server.on("/motor_off", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(relay_pin, LOW);    
    
    // save the LED state in flash memory
    preferences.putBool("state", 0);
    Serial.printf("State saved: 0");
    
    request->send(SPIFFS, "/index.html", String(), false, processor); });

  server.on("/water_level", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", display_waterlevel().c_str()); });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request){
    String inputMessage;
    String inputParam;
    
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      
      Serial.println("depth of the tank is: ");
      Serial.println(inputMessage);
      
      tank_depth = inputMessage.toFloat(); //setting tank depth for program
      preferences.putFloat("depth", tank_depth); //saving depth given by the user
    }

    // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_2)->value();
      inputParam = PARAM_INPUT_2;
      
      Serial.println("Turn off the motor at water level (%): ");
      
      desired_waterlevel = inputMessage.toFloat(); //saving water level
      Serial.println(desired_waterlevel);
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    
    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
                                     + inputParam + ") with value: " + inputMessage +
                                     "<br><a href=\"/\">Return to Home Page</a>"); });

  // Start server
  server.onNotFound(notFound);
  server.begin();
}

void loop()
{
  delay(1000);
}
