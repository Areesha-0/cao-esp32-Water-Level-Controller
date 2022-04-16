void setup() {
  pinMode(2, OUTPUT);//d2 is output pin connected to trig
  pinMode(4, INPUT);//d4 is input pin connected to echo
  Serial.begin(9600);//enables serial monitor to display values

}
void loop() {
  //pulse output
  digitalWrite(2, LOW); //output pin set to low
  delayMicroseconds(2);
  digitalWrite(2, HIGH); //output pin set to high
  delayMicroseconds(10);
  digitalWrite(2, LOW);
  
  long t = pulseIn(4, HIGH);//input pulse time from d4 and save it variable
  
  long cm = t / 29 / 2; //time convert distance
  String CM = " cm";
  
  Serial.println(cm +CM);//print serial monitor cm
  Serial.println();//print space
  delay(1000);//delay
}
