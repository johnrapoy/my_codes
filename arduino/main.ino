#include <Servo.h>
#include "HX711.h"

const int stepperPulsePin = 2;
const int stepperDirPin = 3;
const int stepperEnaPin = 4; 
const int laserTransmitterPin = 5;
const int laserReceiverPin = 6;
const int heightServoPin = 7;
const int proximitySensorPin = 8;
const int rejectServoPin = 9;
const int irSensorFillingPin = 10;
const int shredderRelay = 11;
const int pneumaticRelay = 11;
const int loadcellDoutPin = 12;
const int loadcellSckPin = 13;
const int irSensorCappingPin = 14;
const int cappingRelay = 15;
const int irSensorRejectedPin = 16;


const int ledPin = 4;
const int buzzerPin = 5;



Servo heightServo;
Servo rejectServo;
HX711 scale;

const double calibrationFactor = -7050;
int currentCommand = -1;

void setup() {
  Serial.begin(9600);

  // Conveyor
  pinMode(stepperPulsePin, OUTPUT);
  pinMode(stepperDirPin, OUTPUT);
  pinMode(stepperEnaPin, OUTPUT);

  // Height sensor
  pinMode(laserTransmitterPin, OUTPUT);
  pinMode(laserReceiverPin, INPUT);
  digitalWrite(laserTransmitterPin, HIGH);
  heightServo.attach(heightServoPin);

  // Plastic or can sensor
  pinMode(proximitySensorPin, INPUT);
  rejectServo.attach(rejectServoPin);

  // Filling
  pinMode(irSensorFillingPin, INPUT);
  pinMode(shredderRelay, OUTPUT);
  pinMode(pneumaticRelay, OUTPUT);
  scale.begin(loadcellDoutPin, loadcellSckPin);
  scale.set_scale(calibrationFactor);
  scale.tare();
  while (True){
    if (scale.is_ready()) {
      break;
    }
  }

  // Capping
  pinMode(irSensorCappingPin, INPUT);
  pinMode(cappingRelay, OUTPUT);

  // Rejected container
  pinMode(irSensorRejectedPin, INPUT);
}

void loop() {
  if (currentCommand == -1) {
    receiveCommand();
  }

  // Move conveyor starting
  else if (currentCommand == 0) {
    moveConveyorStart();
    currentCommand = -1;
  }

  // Move conveyor mid
  else if (currentCommand == 1) {
    moveConveyorMid();
    currentCommand = -1;
  }

  // Move conveyor ending
  else if (currentCommand == 2) {
    moveConveyorEnd();
    currentCommand = -1;
  }

  // Detect from laser sensor
  else if (currentCommand == 3) {
    bool detected = digitalRead(laserReceiverPin);
    sendResponse(String(detected));
    currentCommand = -1;
  }

  // Turn on servo motor
  else if (currentCommand == 4) {
    heightServo.write(180);
    currentCommand = -1;
  }

  // Turn off servo motor
  else if (currentCommand == 5) {
    heightServo.write(0);
    currentCommand = -1;
  }

  // Detect from proximity sensor
  else if (currentCommand == 6) {
    bool detected = digitalRead(proximitySensorPin);
    sendResponse(String(!detected));
    currentCommand = -1;
  }

  // Turn on servo motor
  else if (currentCommand == 7) {
    rejectServo.write(180);
    currentCommand = -1;
  }

  // Turn off servo motor
  else if (currentCommand == 8) {
    rejectServo.write(0);
    currentCommand = -1;
  }

  // Detect from IR sensor
  else if (currentCommand == 9) {
    bool detected = digitalRead(irSensorFillingPin);
    sendResponse(String(detected));
    currentCommand = -1;
  }

  // Turn on shredder
  else if (currentCommand == 10) {
    digitalWrite(shredderRelay, HIGH);
    currentCommand = -1;
  }

  // Turn off shredder
  else if (currentCommand == 11) {
    digitalWrite(shredderRelay, LOW);
    currentCommand = -1;
  }

  // Turn on pneumatic actuator
  else if (currentCommand == 12) {
    digitalWrite(pneumaticRelay, HIGH);
    currentCommand = -1;
  }

  // Turn off pneumatic actuator
  else if (currentCommand == 13) {
    digitalWrite(pneumaticRelay, LOW);
    currentCommand = -1;
  }

  // Get weight
  else if (currentCommand == 14) {
    sendResponse(String((scale.get_units(), 1)));
    currentCommand = -1;
  }

  // Detect from IR sensor
  else if (currentCommand == 15) {
    bool detected = digitalRead(irSensorCappingPin);
    sendResponse(String(detected));
    currentCommand = -1;
  }

  // Turn on capping relay
  else if (currentCommand == 16) {
    digitalWrite(cappingRelay, HIGH);
    currentCommand = -1;
  }

  // Turn off capping relay
  else if (currentCommand == 17) {
    digitalWrite(cappingRelay, LOW);
    currentCommand = -1;
  }

  // Detect from IR sensor
  else if (currentCommand == 18) {
    bool detected = digitalRead(irSensorRejectedPin);
    sendResponse(String(detected));
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

void moveConveyorStart() {
  for (int i=0; i<5000; i++) {
    digitalWrite(stepperDirPin,LOW);
    digitalWrite(stepperEnaPin,HIGH);
    digitalWrite(stepperPulsePin,HIGH);
    delayMicroseconds(50);
    digitalWrite(stepperPulsePin,LOW);
    delayMicroseconds(50);
  }
  sendResponse("done");
}

void moveConveyorMid() {
  for (int i=0; i<6000; i++) {
    digitalWrite(stepperDirPin,LOW);
    digitalWrite(stepperEnaPin,HIGH);
    digitalWrite(stepperPulsePin,HIGH);
    delayMicroseconds(50);
    digitalWrite(stepperPulsePin,LOW);
    delayMicroseconds(50);
  }
  sendResponse("done");
}

void moveConveyorEnd() {
  for (int i=0; i<6000; i++) {
    digitalWrite(stepperDirPin,LOW);
    digitalWrite(stepperEnaPin,HIGH);
    digitalWrite(stepperPulsePin,HIGH);
    delayMicroseconds(50);
    digitalWrite(stepperPulsePin,LOW);
    delayMicroseconds(50);
  }
  sendResponse("done");
}
