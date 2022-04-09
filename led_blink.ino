#define LED_BUILTIN 22
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}
void loop() {
  digitalWrite(LED_BUILTIN, HIGH); //led on
  delay(1000); //half a second
  digitalWrite(LED_BUILTIN, LOW); //led off
  delay(1000);
}
