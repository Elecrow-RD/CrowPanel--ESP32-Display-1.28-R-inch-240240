char val;  // Variable to store the read value from the serial buffer

void setup() {
  Serial.begin(115200);  // Initialize the serial communication at a baud rate of 115200
}

void loop() {
  // Check if there is data available in the serial buffer
  if (Serial.available()) {
    val = Serial.read();  // Read the data from the buffer and store it in 'val'
    Serial.print(val);  // Print the read character back to the serial monitor
  }
}
