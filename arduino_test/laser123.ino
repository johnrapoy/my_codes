#include <Servo.h>

// Define laser and sensor pins
#define laser1 3 // Lowest position
#define sensor1 4
#define laser2 5 // Middle position
#define sensor2 6
#define laser3 7 // Highest position
#define sensor3 8

#define servoPin 9 // Define the pin for the servo

Servo myServo; // Create a Servo object

void setup() {
  Serial.begin(9600);

  // Configure laser and sensor pins
  pinMode(laser1, OUTPUT);
  pinMode(sensor1, INPUT);
  pinMode(laser2, OUTPUT);
  pinMode(sensor2, INPUT);
  pinMode(laser3, OUTPUT);
  pinMode(sensor3, INPUT);

  // Turn on lasers
  digitalWrite(laser1, HIGH);
  digitalWrite(laser2, HIGH);
  digitalWrite(laser3, HIGH);

  // Initialize servo
  myServo.attach(servoPin);
  myServo.write(0); // Start the servo at 0 degrees
}

void loop() {
  // Read the state of the sensors
  bool value1 = digitalRead(sensor1); // Laser 1
  bool value2 = digitalRead(sensor2); // Laser 2
  bool value3 = digitalRead(sensor3); // Laser 3

  // Determine the object's height and control the servo
  if (value1 == LOW && value2 == HIGH && value3 == HIGH) {
    // Only laser 1 detects an object
    myServo.write(180); // Rotate the servo to 180 degrees
    Serial.println("Height: Low - Servo rotates to 180");
  } else if (value1 == LOW && value2 == LOW && value3 == HIGH) {
    // Laser 1 and 2 detect an object
    myServo.write(0); // Do not rotate
    Serial.println("Height: Medium - Servo remains at 0");
  } else if (value1 == LOW && value2 == LOW && value3 == LOW) {
    // Laser 1, 2, and 3 detect an object
    myServo.write(180); // Rotate the servo to 180 degrees
    Serial.println("Height: High - Servo rotates to 180");
  } else {
    // No object detected or inconsistent state
    myServo.write(0); // Default position
    Serial.println("No valid height detected - Servo remains at 0");
  }

  delay(500); // Small delay to stabilize readings
}
