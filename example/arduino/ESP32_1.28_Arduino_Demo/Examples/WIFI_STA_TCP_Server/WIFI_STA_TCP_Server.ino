#include <WiFi.h>         // Include the WiFi library for ESP8266 or ESP32
#include <WiFiClient.h>   // Include the WiFiClient library for client functionality

const char* ssid = "elecrow-test";  // The SSID of the WiFi network
const char* password = "1223334444";  // The password of the WiFi network

WiFiServer server(1234);  // Create a WiFiServer object to handle incoming TCP connections on port 1234

void setup() {
  Serial.begin(115200);  // Start the serial communication at a baud rate of 115200
  delay(1000);            // Delay for 1000 milliseconds to ensure the serial port is ready
  WiFi.begin(ssid, password);  // Initiate a connection to the WiFi network
  // Wait for the WiFi connection to be established
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  server.begin();  // Start the server to listen for incoming connections
  Serial.println("WiFi connected");
  Serial.println("IP address: " + WiFi.localIP().toString());  // Print the IP address of the device
}

void loop() {
  // Check if there is a new client that has connected
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connected");
    // Keep the connection open while the client is connected
    while (client.connected()) {
      if (client.available()) {  // Check if there is data available from the client
        String data = client.readStringUntil('\n');  // Read a line of data sent by the client
        Serial.print("Received data: ");
        Serial.println(data);  // Print the received data
        String response = "Server received: " + data;  // Create a response string
        client.println(response);  // Send the response back to the client
      }
    }
    client.stop();  // Close the connection once the client disconnects
    Serial.println("Client disconnected");
  }
}
