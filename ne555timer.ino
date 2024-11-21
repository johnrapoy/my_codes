int ne555 = 7;  // Attach to the third pin of NE555

unsigned long duration1;  // Variable to store the HIGH pulse duration
unsigned long duration2;  // Variable to store the LOW pulse duration
float dc;                 // Variable to store the duty cycle

void setup() {
  pinMode(ne555, INPUT);   // Set the ne555 pin as input
  Serial.begin(9600);      // Start serial port at 9600 bps
}

void loop() {
  // Measure the HIGH and LOW pulse durations
  duration1 = pulseIn(ne555, HIGH);  // Reads the HIGH pulse on ne555
  duration2 = pulseIn(ne555, LOW);   // Reads the LOW pulse on ne555

  // Calculate the duty cycle as a percentage
  if (duration1 + duration2 > 0) {   // Avoid division by zero
    dc = (duration1 * 100.0) / (duration1 + duration2);
  } else {
    dc = 0;  // If no pulse detected, set duty cycle to 0
  }

  // Print the duty cycle to the Serial Monitor
  Serial.print("Duty cycle: ");
  Serial.print(dc);  // Print the calculated duty cycle
  Serial.println(" %");  // Print the percentage symbol

  delay(500);  // Wait for 500 milliseconds
}
