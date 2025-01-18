#include <Servo.h>
#include "HX711.h"

const int stepperPulsePin = 2;
const int stepperDirPin = 3;
const int stepperEnaPin = 4;
const int irSensorBottlePin = 5;
const int irSensorPin1 = 6;
const int irSensorPin2 = 7;
const int irSensorPin3 = 8;
const int heightServoPin = 9;
const int proximitySensorPin = 10;
const int fillingServoPin = 11;
const int pneumaticRelay = 12;
const int loadcellDoutPin = 17;
const int loadcellSckPin = 18;

const int ledPin = 4;
const int buzzerPin = 5;

Servo fillingServo;
Servo heightServo;
Servo rejectServo;
HX711 scale;

const double calibrationFactor = 356.634002;
int currentCommand = -1;

void setup() {
  Serial.begin(9600);

  // Conveyor
  pinMode(stepperPulsePin, OUTPUT);
  pinMode(stepperDirPin, OUTPUT);
  pinMode(stepperEnaPin, OUTPUT);

  // Bottle Detection
  pinMode(irSensorBottlePin, INPUT);

  // Height sensor
  pinMode(irSensorPin1, INPUT);
  pinMode(irSensorPin2, INPUT);
  pinMode(irSensorPin3, INPUT);
  heightServo.attach(heightServoPin);

  // Plastic or can sensor
  pinMode(proximitySensorPin, INPUT);
  heightServo.write(180);

  fillingServo.write(0);

  // Filling
  pinMode(pneumaticRelay, OUTPUT);
  fillingServo.attach(fillingServoPin);
  scale.begin(loadcellDoutPin, loadcellSckPin);
  scale.set_scale(calibrationFactor);
  scale.tare();

  // Wait for the scale to be ready
  while (true) {
    if (scale.is_ready()) {
      break;
    }
  }
} // End of setup()

void loop() {
  if (currentCommand == -1) {
    receiveCommand();
  }

  // Move conveyor starting
  else if (currentCommand == 0) {
    moveConveyorStart();
    currentCommand = -1;
  }

  // Move conveyor reject
  else if (currentCommand == 1) {
    moveConveyorReject();
    currentCommand = -1;
  }

  // Move conveyor mid
  else if (currentCommand == 2) {
    moveConveyorMid();
    currentCommand = -1;
  }

  // Move conveyor ending
  else if (currentCommand == 3) {
    moveConveyorEnd();
    currentCommand = -1;
  }

  // Move conveyor ending
  else if (currentCommand == 4) {
    moveConveyorFinal();
    currentCommand = -1;
  }

  // Detect from IR Sensor
  else if (currentCommand == 5) {
    bool detected = digitalRead(irSensorBottlePin);
    sendResponse(String(!detected));
    currentCommand = -1;
  } 

  // Detect height based on ir sensors
  else if (currentCommand == 6) {
    bool value1 = digitalRead(irSensorPin1); // Corrected laserReceiverPin1 to irSensorPin1
    bool value2 = digitalRead(irSensorPin2);
    bool value3 = digitalRead(irSensorPin3);
    bool response = false;

    // Adjusted logic for reversed detection states
    if (!value1 && !value2 && value3) {
      response = true;
    }

    sendResponse(String(response));
    currentCommand = -1;
  }

  // Turn off servo motor
  else if (currentCommand == 7) {
    heightServo.write(180);
    currentCommand = -1;
  }

  // Turn on servo motor
  else if (currentCommand == 8) {
    heightServo.write(0);
    currentCommand = -1;
  }

  // Detect from proximity sensor
  else if (currentCommand == 9) {
    bool detected = digitalRead(proximitySensorPin);
    sendResponse(String(!detected));
    currentCommand = -1;
  }

  // Turn on servo motor
  else if (currentCommand == 10) {
    fillingServo.write(180);
    currentCommand = -1;
  }

  // Turn off servo motor
  else if (currentCommand == 11) {
    fillingServo.write(0);
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
    sendResponse(String(scale.get_units(), 1));
    currentCommand = -1;
  }
}

void receiveCommand() {
  if (Serial.available()) {
    int sent = Serial.readStringUntil('\n').toInt();
    Serial.println("ok");
    currentCommand = sent;
  }
}

void sendResponse(String response) {
  Serial.println(response);    
}

void moveConveyorStart() {
  long signed iterations = 33950;
  for (long signed i = 0; i < iterations; i++) {
    digitalWrite(stepperDirPin, LOW);
    digitalWrite(stepperEnaPin, HIGH);
    digitalWrite(stepperPulsePin, HIGH);
    delayMicroseconds(50);
    digitalWrite(stepperPulsePin, LOW);
    delayMicroseconds(50);
  }
  sendResponse("done");
}

void moveConveyorReject() {
  long signed iterations = 5000;
  for (long signed i = 0; i < iterations; i++) {
    digitalWrite(stepperDirPin, LOW);
    digitalWrite(stepperEnaPin, HIGH);
    digitalWrite(stepperPulsePin, HIGH);
    delayMicroseconds(50);
    digitalWrite(stepperPulsePin, LOW);
    delayMicroseconds(50);
  }
  sendResponse("done");
}

void moveConveyorMid() {
  long signed iterations = 70000;
  for (long signed i = 0; i < iterations; i++) {
    digitalWrite(stepperDirPin, LOW);
    digitalWrite(stepperEnaPin, HIGH);
    digitalWrite(stepperPulsePin, HIGH);
    delayMicroseconds(50);
    digitalWrite(stepperPulsePin, LOW);
    delayMicroseconds(50);
  }
  sendResponse("done");
}

void moveConveyorEnd() {
  long signed iterations = 35000;
  for (long signed i = 0; i < iterations; i++) {
    digitalWrite(stepperDirPin, LOW);
    digitalWrite(stepperEnaPin, HIGH);
    digitalWrite(stepperPulsePin, HIGH);
    delayMicroseconds(50);
    digitalWrite(stepperPulsePin, LOW);
    delayMicroseconds(50);
  }
  sendResponse("done");
}

void moveConveyorFinal() {
  long signed iterations = 35000;
  for (long signed i = 0; i < iterations; i++) {
    digitalWrite(stepperDirPin, LOW);
    digitalWrite(stepperEnaPin, HIGH);
    digitalWrite(stepperPulsePin, HIGH);
    delayMicroseconds(50);
    digitalWrite(stepperPulsePin, LOW);
    delayMicroseconds(50);
  }
  sendResponse("done");
}
