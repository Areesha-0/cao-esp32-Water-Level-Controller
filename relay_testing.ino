const int relay = 26;
void setup()
{     
  Serial.begin(115200);
//  Blynk.begin(auth, ssid, pass);    
  pinMode(relay,OUTPUT);
}

void loop()
{
  //Using normally closed configuration on relay 
  digitalWrite(relay, LOW);
  Serial.print("Current not flowing");

  delay(1000); //turns off for a second

  digitalWrite(relay, HIGH);
  Serial.print("Current flowing");

  delay(5000); //turns on for five seconds
 }
