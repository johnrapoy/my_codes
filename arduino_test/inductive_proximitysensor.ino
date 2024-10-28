#define laser 1
#define sensor 2
#define LED 3
#define buzzer 4

void setup() {
  Serial.begin(9600);
  pinMode(laser, OUTPUT);
  pinMode(sensor, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(buzzer, OUTPUT);

  digitalWrite(laser, HIGH);
}

void loop() {
  bool value = digitalRead(sensor);

  if (value == 0) {
    digitalWrite(LED, HIGH);
    noTone(buzzer); // 1000 Hz tone for the buzzer
  } else {
    digitalWrite(LED, LOW);
    Tone(buzzer, 500); // Stop the tone on the buzzer
  }
}
