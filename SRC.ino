/*
A laser pointer is needed to make a light beam that will be detected by the ldr
Make Sure you line up the beam and the ldr properly
*/
const int sensivity = 400;  // 0-1023 a lower value means a more powerful laser

#define ldr A0
#define redLed 7
#define blueLed 8
#define buzzer 9
#define buttonPin A2


int buttonState;
int state = 0;


void setup() {
  alarmOFF();
  Serial.begin(9600);
  pinMode(redLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  int light = analogRead(ldr);
  int button = analogRead(buttonPin);
  Serial.print("button = ");
  Serial.print(button);
  Serial.print("\n");  
  Serial.print("light = ");
  Serial.print(light);
  Serial.print("\n");  
  Serial.print("state = ");
  Serial.print(state);
  Serial.print("\n"); 
  Serial.print("\n");  
  if (button >= 1020) {
    state = 0;
  }
  if (light > sensivity) {
    state = 1;
  }
  if ((light <= sensivity) && (state == 0)) {
    alarmOFF();
  }
  if (state == 1) {
    alarmON();
  }
}


void alarmON() {
  tone(buzzer, 1760);
  digitalWrite(redLed, HIGH);
  digitalWrite(blueLed, LOW);
  delay(500);
  tone(buzzer, 1319);
  digitalWrite(blueLed, HIGH);
  digitalWrite(redLed, LOW);
  delay(500);
}

void alarmOFF() {
  noTone(buzzer);
  digitalWrite(redLed, LOW);
  digitalWrite(blueLed, LOW);
}
