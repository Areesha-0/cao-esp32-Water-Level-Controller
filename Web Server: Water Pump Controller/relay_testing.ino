// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"

// Assign each GPIO to a relay
int relayGPIO = 13;

// Replace with your network credentials
const char* ssid = "muhammadasif7";
const char* password = "asif1972";
 
const char* PARAM_INPUT_2 = "state";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 34px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
    input:checked+.slider {background-color: #2196F3}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>
<body>
  <h2>ESP Web Server</h2>
  %BUTTONPLACEHOLDER%
<script>function toggleCheckbox() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/update?relay_state=1", true); 
  xhr.send();
}</script>
</body>
</html>
)rawliteral";

String relayState(){
  if(digitalRead(relayGPIO)){
    return "checked";
  }
  else{
    return "";
  }
  return "";
}
// Replaces placeholder with button section in your web page
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

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

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

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
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
    request->send(200, "text/plain", "OK");
  });
  // Start server
  server.begin();
}
  
void loop() {
}
