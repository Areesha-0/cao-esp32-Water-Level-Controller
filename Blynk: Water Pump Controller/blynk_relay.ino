#define BLYNK_TEMPLATE_ID "TMPLg1OxXrwX"
#define BLYNK_DEVICE_NAME "Water Pump Controller"
#define BLYNK_AUTH_TOKEN "LhbCoP2ExeHueOYjSJqppGTrM0F0dAfo"


#define BLYNK_PRINT Serial
#include <WiFi.h> 
 
#include <BlynkSimpleEsp32.h>
 

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "muhammadasif7";  // Enter your wifi name
char pass[] = "asif1972";  // Enter your wifi password

const int relay = 13;
void setup()
{     
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);    
  pinMode(relay,OUTPUT);
}

void loop()
{
  Blynk.run();
 }
