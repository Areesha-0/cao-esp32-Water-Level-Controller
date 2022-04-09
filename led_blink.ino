#define LED_BUILTIN 22 //gpio pin 22 used for led
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}
void loop() {
  digitalWrite(LED_BUILTIN, HIGH); //led on
  delay(1000); //a second
  digitalWrite(LED_BUILTIN, LOW); //led off
  delay(1000);
}
