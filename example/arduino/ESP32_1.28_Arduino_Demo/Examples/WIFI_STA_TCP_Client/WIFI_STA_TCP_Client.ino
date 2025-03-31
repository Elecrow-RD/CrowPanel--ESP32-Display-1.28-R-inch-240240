#include <WiFi.h>

const char *ssid = "elecrow-test";  // The SSID of the WiFi network to connect to
const char *password = "1223334444";  // The password of the WiFi network

const IPAddress serverIP(192, 168, 3, 105);  // The IP address of the server to connect to
uint16_t serverPort = 9527;  // The port number on the server to connect to

WiFiClient client;  // Create an instance of the WiFiClient class to create a TCP connection

void setup() {
  Serial.begin(115200);  // Start the serial communication at a baud rate of 115200
  delay(1000);  // Delay for 1000 milliseconds to give the serial console time to initialize
  WiFi.begin(ssid, password);  // Initiate a connection to the WiFi network
  // Wait for the WiFi connection to be established
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected");  // Print a message to indicate the connection is established
  Serial.print("IP Address:");  // Print the IP address assigned to the device
  Serial.println(WiFi.localIP());
}

void loop() {
  // Attempt to establish a TCP connection with the server
  if (client.connect(serverIP, serverPort)) {
    client.print("Hello world!");  // Send a "Hello world!" message to the server
    // Keep the connection open and interact with the server
    while (client.connected() || client.available()) {
      if (client.available()) {  // Check if there is data available from the server
        String line = client.readStringUntil('\n');  // Read a line of data from the server
        Serial.print("Read data:");  // Print a message indicating data was read
        Serial.println(line);
        client.write(line.c_str());  // Echo the received data back to the server
      }
    }
    Serial.println("Close Current Connection");  // Print a message indicating the connection is closed
    client.stop();  // Close the TCP connection
  } else {
    Serial.println("Access Failure");  // Print a message indicating the connection failed
    client.stop();  // Ensure the client stops any attempt to connect
  }
  delay(5000);  // Wait for 5000 milliseconds before attempting to reconnect
}
