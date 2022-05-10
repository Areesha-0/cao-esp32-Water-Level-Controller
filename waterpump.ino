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
long tank_depth = 100;
double water_level;
const int trigPin = 33;
const int echoPin = 32;
const int relay = 13;
int relay_state = 0; //turns off at zero

BLYNK_WRITE(V1){
  relay_state = param.asInt();
  digitalWrite(relay, relay_state);
}

void ultrasonic_sensor(){
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
  water_level = tank_depth - wave_distance;
  water_level = (water_level / tank_depth) * 100 ; //calculating percentage of water present in the tank
  
  // Prints the distance from water level to top of tank and water level on the Serial Monitor
  Serial.print("tank height left (cm): ");
  Serial.println(wave_distance);
  Serial.print("water level in %: ");
  Serial.println(water_level);
  
  //turns relay off if the tank is about to be full automatically !but not the button on blynk!
  if(water_level >= 95){
    Serial.print("tank is full");
    digitalWrite(relay, LOW);
    Blynk.virtualWrite(V1, 0);
  }
 
  Blynk.virtualWrite(V0, water_level);
  delay(1000);
}

//updates latest virtual pin value stored on server 
BLYNK_CONNECTED(){
  Blynk.syncVirtual(V0);
  Blynk.syncVirtual(V1);
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
