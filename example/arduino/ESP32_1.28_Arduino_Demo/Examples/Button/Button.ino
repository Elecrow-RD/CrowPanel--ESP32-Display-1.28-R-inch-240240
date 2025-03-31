// Define the pin number for the button
const int buttonPin = 1;  // The pin number where the button is connected

// Variable to store the current state of the button
int buttonState = 0;  // Variable to hold the button's state, initially set to HIGH (0)

void setup() {
  Serial.begin(115200);  // Initialize serial communication at a baud rate of 115200
  pinMode(buttonPin, INPUT);  // Set the button pin as an input
}

void loop() {
  // Read the current state of the button (HIGH or LOW)
  buttonState = digitalRead(buttonPin);
  
  // If the button is pressed (connected to ground, state is LOW)
  if (buttonState == LOW) {
    Serial.println("Button Press");  // Print a message when the button is pressed
  } 
  // If the button is not pressed, it is in the release state (HIGH)
  else {
    Serial.println("Button release");  // Print a message when the button is released
  }
  
  // Note: This loop will continuously read the button state and print messages.
  // This may not be the desired behavior in all applications.
  // Consider adding a delay or using a debouncing method for the button.
}
