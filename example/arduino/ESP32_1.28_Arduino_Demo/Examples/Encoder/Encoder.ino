// Encoder defines
#define ENCODER_A_PIN 19  // Pin definition for Encoder input A
#define ENCODER_B_PIN 18  // Pin definition for Encoder input B
#define SWITCH_PIN    8    // Pin definition for Switch

bool switchPressed = false;  // Variable to track switch state
long position_tmp = 0;       // Temporary variable to hold encoder position

// Encoder update function to track changes in encoder state
void updateEncoder() {
  static int previousState = 0;  // Hold the previous state of the encoder pins
  static int flag_A = 0;         // Counter for 'forward' direction
  static int flag_C = 0;         // Counter for 'reversal' direction

  int currentState = (digitalRead(ENCODER_A_PIN) << 1) | digitalRead(ENCODER_B_PIN);  // Read the current state of the encoder

  // Check for 'forward' direction logic
  if ((currentState == 0b00 && previousState == 0b01) || 
      (currentState == 0b01 && previousState == 0b11) || 
      (currentState == 0b11 && previousState == 0b10) || 
      (currentState == 0b10 && previousState == 0b00)) {
    flag_A++;
    if (flag_A == 50) {
      flag_A = 0;
      flag_C = 0;
      position_tmp = 1;
      Serial.println("forward");  // Print out the direction
    }
  } 
  // Check for 'reversal' direction logic
  else if ((currentState == 0b01 && previousState == 0b00) || 
           (currentState == 0b11 && previousState == 0b01) || 
           (currentState == 0b10 && previousState == 0b11) || 
           (currentState == 0b00 && previousState == 0b10)) {
    flag_C++;
    if (flag_C == 50) {
      flag_C = 0;
      flag_A = 0;
      position_tmp = 0;
      Serial.println("reversal");  // Print out the direction
    }
  }

  previousState = currentState;  // Update the previous state
}

// Interrupt function for switch press
void switchPressedInterrupt() {
  switchPressed = !switchPressed;  // Toggle the switchPressed state
  if (switchPressed) {
    Serial.println("press");  // Indicate switch press
  } else {
    Serial.println("release");  // Indicate switch release
  }
}

void setup() {
  Serial.begin(115200);  // Start serial communication
  pinMode(ENCODER_A_PIN, INPUT_PULLUP);  // Set encoder pins as input with pull-up
  pinMode(ENCODER_B_PIN, INPUT_PULLUP);
  pinMode(SWITCH_PIN, INPUT_PULLUP);

  // Attach interrupts for encoder and switch
  attachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_B_PIN), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), switchPressedInterrupt, FALLING);
}

void loop() {
  // Main loop, currently does nothing but wait
  delay(10);
}
