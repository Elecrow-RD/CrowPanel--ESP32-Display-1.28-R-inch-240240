#include "WiFi.h"  // Include the WiFi library for ESP32

void setup() {
  Serial.begin(115200);  // Start the serial communication at a baud rate of 115200
  WiFi.softAP("ESP32C3_AP", "12345678");  // Set up the ESP32 as an Access Point with the SSID "ESP32C3_AP" and password "12345678"
}

void loop() {
  Serial.print("Host Name:");  // Print the host name of the ESP32
  Serial.println(WiFi.softAPgetHostname());
  Serial.print("Host IP Address:");  // Print the IP address of the Access Point
  Serial.println(WiFi.softAPIP());
  Serial.print("Host IPV6:");  // Print the IPv6 address of the Access Point
  Serial.println(WiFi.softAPIPv6());
  Serial.print("Host SSID:");  // Print the SSID of the Access Point
  Serial.println(WiFi.SSID());
  Serial.print("Host Broadcast IP:");  // Print the broadcast IP address of the Access Point
  Serial.println(WiFi.softAPBroadcastIP());
  Serial.print("Host Mac Address:");  // Print the MAC address of the Access Point
  Serial.println(WiFi.softAPmacAddress());
  Serial.print("Number Of Host Connections:");  // Print the number of devices connected to the Access Point
  Serial.println(WiFi.softAPgetStationNum());
  Serial.print("Host Network ID:");  // Print the network ID
  Serial.println(WiFi.softAPNetworkID());
  Serial.print("Host State:");  // Print the current state of the WiFi host (Access Point)
  Serial.println(WiFi.status());
  delay(1000);  // Wait for 1 second before printing the information again
}
