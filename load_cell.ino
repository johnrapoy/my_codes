#include "HX711.h"

#define calibration_factor -7050.0 // Obtained using the SparkFun_HX711_Calibration sketch

#define LOADCELL_DOUT_PIN  3
#define LOADCELL_SCK_PIN  2

HX711 scale;

void setup() {
  Serial.begin(9600);
  Serial.println("HX711 scale demo");

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor); // Set calibration factor
  scale.tare(); // Reset the scale to 0

  Serial.println("Readings:");
  
  // Simple test to check if the scale is working
  if (scale.is_ready()) {
    Serial.println("Scale is ready.");
  } else {
    Serial.println("Error: HX711 not found.");
    while (1); // Stop the code if the scale isn't connected properly
  }
}

void loop() {
  // Continuously print weight readings
  Serial.print("Reading: ");
  Serial.print(scale.get_units(), 1); // Returns weight in pounds
  Serial.print(" lbs");
  Serial.println();

  delay(1000); // Adding a delay for better readability of output
}