// Define the pin number for the buzzer
#define BUZZER 3  

void setup() {
  // This code runs once at the beginning of the program:
  pinMode(BUZZER, OUTPUT);  // Set the buzzer pin as an output
  digitalWrite(BUZZER, LOW);  // Initially set the buzzer to a low state (off)
}

void loop() {
  // This code runs repeatedly after the setup():
  tone(BUZZER, 100);  // Make the buzzer produce a tone at a frequency of 100 Hz
  delay(1000);  // Wait for one second
  tone(BUZZER, 0);  // Stop the buzzer from producing sound (frequency set to 0)
  delay(1000);  // Wait for another second
}
