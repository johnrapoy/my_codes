#include <Servo.h>

const int laserTransmitterPin = 3;
const int laserReceiverPin = 4;
const int ledPin = 5;
const int buzzerPin = 6;
const int proximitySensorPin = 7;
const int servoPin = 8;
const int irSensorPin = 9;
const int stepperPulsePin = 10;
const int stepperDirPin = 11;
const int stepperEnaPin = 12; 


Servo servo;

int currentCommand = -1;

void setup() {
  Serial.begin(9600);
  pinMode(laserTransmitterPin, OUTPUT);
  pinMode(laserReceiverPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(proximitySensorPin, INPUT);
  pinMode(irSensorPin, INPUT);
  pinMode(stepperPulsePin, OUTPUT);
  pinMode(stepperDirPin, OUTPUT);
  pinMode(stepperEnaPin, OUTPUT);

  digitalWrite(laserTransmitterPin, HIGH);
  servo.attach(servoPin);
}

void loop() {
  if (currentCommand == -1) {
    receiveCommand();
  }

  else if (currentCommand == 0) {
    // Detect from laser sensor
    bool detected = digitalRead(sensor);
    sendResponse(String(detected));
    currentCommand = -1;
  }

  else if (currentCommand == 1) {
    // Turn on LED
    digitalWrite(ledPin, HIGH);
    currentCommand = -1;
  }

  else if (currentCommand == 2) {
    // Turn off LED
    digitalWrite(ledPin, LOW);
    currentCommand = -1;
  }

  else if (currentCommand == 3) {
    // Turn on buzzer
    tone(buzzerPin, 500);
    currentCommand = -1;
  }

  else if (currentCommand == 4) {
    // Turn off buzzer
    noTone(buzzerPin);
    currentCommand = -1;
  }

  else if (currentCommand == 5) {
    // Detect from proximity sensor
    bool detected = digitalRead(proximitySensorPin);
    sendResponse(String(detected));
    currentCommand = -1;
  }

  else if (currentCommand == 6) {
    // Turn on servo motor
    servo.write(180);
    currentCommand = -1;
  }

  else if (currentCommand == 7) {
    // Turn off servo motor
    servo.write(0);
    currentCommand = -1;
  }

  else if (currentCommand == 8) {
    // Detect from IR sensor
    bool detected = digitalRead(irSensorPin);
    sendResponse(String(detected));
    currentCommand = -1;
  }

  else if (currentCommand == 9) {
    // Move conveyor
    moveConveyor();
    currentCommand = -1;
  }
}

void receiveCommand() {
  if(Serial.available()) {
    int sent = Serial.readStringUntil('\n').toInt();
    Serial.println("ok");
    currentCommand = sent;
  }
}

void sendResponse(String response) {
  Serial.println(response);    
}

void moveConveyor() {
  for (int i=0; i<7500; i++) {
    digitalWrite(stepperDirPin,LOW);
    digitalWrite(stepperEnaPin,HIGH);
    digitalWrite(stepperPulsePin,HIGH);
    delayMicroseconds(50);
    digitalWrite(stepperPulsePin,LOW);
    delayMicroseconds(50);
  }
  sendResponse("done");
}
