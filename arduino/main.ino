#include <Servo.h>
#include "HX711.h"

const int stepperPulsePin = 2;
const int stepperDirPin = 3;
const int stepperEnaPin = 4;
const int irSensorBottlePin = 4;
const int laserTransmitterPin1 = 5;
const int laserReceiverPin1 = 6;
const int laserTransmitterPin2 = 7;
const int laserReceiverPin2 = 8;
const int laserTransmitterPin3 = 9;
const int laserReceiverPin3 = 10;
const int heightServoPin = 11;
const int proximitySensorPin = 12;
const int irSensorFillingPin = 14;
const int fillingServoPin = 11;
const int pneumaticRelay = 16;
const int loadcellDoutPin = 17;
const int loadcellSckPin = 18;


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

  // Bottle Detection
  pinMode(irSensorBottlePin, INPUT);
}

  // Height sensor
  pinMode(laserTransmitterPin1, OUTPUT);
  pinMode(laserReceiverPin1, INPUT);
  pinMode(laserTransmitterPin2, OUTPUT);
  pinMode(laserReceiverPin2, INPUT);
  pinMode(laserTransmitterPin3, OUTPUT);
  pinMode(laserReceiverPin3, INPUT);
  digitalWrite(laserTransmitterPin1, HIGH);
   digitalWrite(laserTransmitterPin2, HIGH);
   digitalWrite(laserTransmitterPin3, HIGH);
  heightServo.attach(heightServoPin);

  // Plastic or can sensor
  pinMode(proximitySensorPin, INPUT);
  heightServo.attach(heightServoPin);

  // Filling
  pinMode(irSensorFillingPin, INPUT);
  pinMode(shredderRelay, OUTPUT);
  pinMode(pneumaticRelay, OUTPUT);
  fillingServoPin.attach(fillingServoPin);
  scale.begin(loadcellDoutPin, loadcellSckPin);
  scale.set_scale(calibrationFactor);
  scale.tare();
  while (True){
    if (scale.is_ready()) {
      break;
    }
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

  // Bottle Detected
  else if 

  
// Detect height based on laser sensors
if (currentCommand == 3) {
  bool value1 = digitalRead(laserReceiverPin1); 
  bool value2 = digitalRead(laserReceiverPin2); 
  bool value3 = digitalRead(laserReceiverPin3); 

  String response;
  if (value1 == LOW && value2 == HIGH && value3 == HIGH) {
    response = "Height: Low - Laser 1 detects object";
  } else if (value1 == LOW && value2 == LOW && value3 == HIGH) {
    response = "Height: Medium - Laser 1 and 2 detect object";
  } else if (value1 == LOW && value2 == LOW && value3 == LOW) {
    response = "Height: High - Laser 1, 2, and 3 detect object";
  } else {
    response = "No valid height detected";
  }

  sendResponse(response);
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
