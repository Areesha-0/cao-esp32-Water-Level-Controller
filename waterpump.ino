#define BLYNK_TEMPLATE_ID "TMPLg1OxXrwX"
#define BLYNK_DEVICE_NAME "Water Pump Controller"
#define BLYNK_AUTH_TOKEN "LhbCoP2ExeHueOYjSJqppGTrM0F0dAfo"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
char ssid[] = "muhammadasif7";
char pass[] = "asif1972";

//define sound speed in cm/uS
#define SOUND_SPEED 0.034

long duration;
long wave_distance; //in centimeter
long tank_depth = 0;
double water_level;
const int trigPin = 33;
const int echoPin = 32;
const int relay = 13;
int relay_state = 0; //turns off at zero
int button_state = 0; //turns off at zero
int tanklevel_50 = 0; //turns off at zero
int tanklevel_30 = 0; //turns off at zero
int tanklevel_75 = 0; //turns off at zero

BLYNK_WRITE(V1){
  relay_state = param.asInt();
  digitalWrite(relay, relay_state);
}

BLYNK_WRITE(V2){
  button_state = param.asInt(); //button for measuring tank's height
}

BLYNK_WRITE(V3){
  tanklevel_50 = param.asInt(); //button for turning the tank on/off at 50% water level
  digitalWrite(relay, tanklevel_50);
}

BLYNK_WRITE(V4){
  tanklevel_30 = param.asInt(); //button for turning the tank on/off at 30% water level
  digitalWrite(relay, tanklevel_30);
}

BLYNK_WRITE(V5){
  tanklevel_75 = param.asInt(); //button for turning the tank on/off at 75% water level
  digitalWrite(relay, tanklevel_75);
}

void ultrasonic_sensor(){

 Blynk.logEvent("Calibrate the depth of your water tank");
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  wave_distance = duration * SOUND_SPEED/2; //distance (cm) from water level to the top of the tank

  //setting depth of the tank
  if(button_state == 1) {
        int measured_tankDepth = wave_distance; //tank height measurement if its empty
        tank_depth = measured_tankDepth;
        Serial.println(tank_depth);
        if(button_state == 0) Blynk.virtualWrite(V2,0);
  }
  
  else{
        water_level = tank_depth - wave_distance;
        water_level = (water_level / tank_depth) * 100 ; //calculating percentage of water present in the tank
        
        // Prints the distance from water level to top of tank and water level on the Serial Monitor
        Serial.print("tank depth: ");
        Serial.println(tank_depth);
        Serial.print("tank height left (cm): ");
        Serial.println(wave_distance);
        Serial.print("water level in %: ");
        Serial.println(water_level);
        
        //turns relay off if the tank is about to be full automatically
        if(water_level >= 95){
          Serial.print("Tank is full");
          digitalWrite(relay, LOW);
          Blynk.virtualWrite(V1, 0); //shows switch off on the app
          Blynk.logEvent("Water tank is about to overflow");
        }

        if(water_level == 0) Blynk.logEvent("Your tank is empty");

        if(water_level <= 10 && water_level > 0) Blynk.logEvent("Your tank is almost empty");
        
        if(tanklevel_30 == 1 && water_level >= 30) {
           Blynk.virtualWrite(V4,0); //button also turns off on the app
           tanklevel_30 = 0;
           digitalWrite(relay, tanklevel_30); //water pump turned off on said percetage of water in the tank
           Serial.print("Tank is 30% full");
        }
        
        if(tanklevel_50 == 1 && water_level >= 50){
            Blynk.virtualWrite(V3,0); //button also turns off on the app
            tanklevel_50 = 0;
            digitalWrite(relay, tanklevel_50); //water pump turned off on said percetage of water in the tank
            Serial.print("Tank is 50% full");
            Blynk.logEvent("50 percent of your tank is full");
        }
          
        if(tanklevel_75 == 1 && water_level >= 75) {
           Blynk.virtualWrite(V5,0); //button also turns off on the app
           tanklevel_75 = 0;
           digitalWrite(relay, tanklevel_75); //water pump turned off on said percetage of water in the tank
           Serial.print("Tank is 75% full");
           Blynk.logEvent("75 percent of your tank is full");
        }

       
        Blynk.virtualWrite(V0, water_level);
        delay(1000);
        }
}

//updates latest virtual pin value stored on server 
BLYNK_CONNECTED(){
  Blynk.syncVirtual(V0);
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V2);
  Blynk.syncVirtual(V3);
  Blynk.syncVirtual(V4);
  Blynk.syncVirtual(V5);
}

void setup() {
  Serial.begin(115200); // Starts the serial communication
  Blynk.begin(auth, ssid, pass);
  pinMode(relay,OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

void loop() {
  Blynk.run();
  ultrasonic_sensor();
}
