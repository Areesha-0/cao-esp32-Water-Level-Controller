//testing the 5v relay 
bool pinValue = 0;
#define relay D5
 
void setup() {
  pinMode(relay, OUTPUT); //setting pin
  Serial.begin(9600);
}

void Wlevel() {
  //relay on and off working
  if (pinValue == 1) { 
    digitalWrite(relay, LOW);
  } else if (pinValue == 0) {
    digitalWrite(relay, HIGH);
  }
  Serial.println(cm);
}
