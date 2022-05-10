#define BLYNK_TEMPLATE_ID "TMPLiyu911v2"
#define BLYNK_DEVICE_NAME "sensor"
#define BLYNK_AUTH_TOKEN "ZOLfeunxXICJTMzCY8uLr_94KSKAcE4Z"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
char ssid[] = "muhammadasif7";
char pass[] = "asif1972";

const int trigPin = 33;
const int echoPin = 32;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
int distance; //in centimeter

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
  
  // Calculate the distance -> cm
  distance = duration * SOUND_SPEED/2;

  if(distance == 7)
  {
    Serial.print("tank is full");
  }
 
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distance);

    Blynk.virtualWrite(V0, distance);
  
  delay(1000);
}
void setup() {
  Serial.begin(115200); // Starts the serial communication
  Blynk.begin(auth, ssid, pass);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

void loop() {
  Blynk.run();
  ultrasonic_sensor();
}
