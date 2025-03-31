#include "WiFi.h"  // Include the WiFi library for ESP8266 or ESP32

void setup() {
  Serial.begin(115200);  // Initialize serial communication at a baud rate of 115200

  // Start WiFi connection with given SSID and password
  WiFi.begin("elecrow-test", "1223334444");
  // Enable auto-reconnect feature
  WiFi.setAutoReconnect(true);
}

void loop() {
  // Print the connection status: true if connected, false otherwise
  Serial.print("Connection Status:");
  Serial.println(WiFi.isConnected());
  // Print the local IP address of the device
  Serial.print("Local IP:");
  Serial.println(WiFi.localIP());
  // Print the local IPv6 address of the device
  Serial.print("Local IPv6:");
  Serial.println(WiFi.localIPv6());
  // Print the MAC address of the device
  Serial.print("Mac Address:");
  Serial.println(WiFi.macAddress());
  // Print the network ID
  Serial.print("Network ID:");
  Serial.println(WiFi.networkID());
  // Print the sleep mode setting
  Serial.print("Sleep:");
  Serial.println(WiFi.getSleep());
  // Print the WiFi status bits
  Serial.print("Get Status:");
  Serial.println(WiFi.getStatusBits());
  // Print the transmission power setting
  Serial.print("Get Tx Power:");
  Serial.println(WiFi.getTxPower());
  // Print the auto connect setting
  Serial.print("get Auto Connect:");
  Serial.println(WiFi.getAutoConnect());
  // Print the auto reconnect setting
  Serial.print("Get Auto Reconnect:");
  Serial.println(WiFi.getAutoReconnect());
  // Print the current WiFi mode (1 - WIFI_OFF, 2 - WIFI_STA, 3 - WIFI_AP, 4 - WIFI_AP_STA)
  Serial.print("Get Mode:");
  Serial.println(WiFi.getMode());
  // Print the device's hostname
  Serial.print("Get Hostname:");
  Serial.println(WiFi.getHostname());
  // Print the gateway IP address
  Serial.print("Gateway IP:");
  Serial.println(WiFi.gatewayIP());
  // Print the DNS server IP address
  Serial.print("dns IP:");
  Serial.println(WiFi.dnsIP());
  // Print the current WiFi status
  Serial.print("Status:");
  Serial.println(WiFi.status());
  delay(1000);  // Wait for 1 second before printing the information again
}
