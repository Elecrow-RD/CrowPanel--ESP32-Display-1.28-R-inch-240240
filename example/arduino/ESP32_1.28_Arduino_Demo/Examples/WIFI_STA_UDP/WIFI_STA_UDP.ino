#include <WiFi.h>
#include <AsyncUDP.h> 

const char *ssid = "elecrow-test";  // SSID of the WiFi network
const char *password = "1223334444";  // Password of the WiFi network

AsyncUDP udp;  // Create an AsyncUDP object for handling UDP packets
unsigned int localUdpPort = 9527;  // Local UDP port number

unsigned int broadcastPort = localUdpPort;  // Port number for broadcasting
const char *broadcastData = "from esp32c3";  // Data to be sent as broadcast

// Callback function for handling incoming UDP packets
void onPacketCallBack(AsyncUDPPacket packet) {
  Serial.print("UDP packet source type: ");
  Serial.println(packet.isBroadcast() ? "broadcast data" : (packet.isMulticast() ? "Multicast" : "Unicast"));
  Serial.print("Remote address and port number: ");
  Serial.print(packet.remoteIP());
  Serial.print(":");
  Serial.println(packet.remotePort());
  Serial.print("Destination address and port number: ");
  Serial.print(packet.localIP());
  Serial.print(":");
  Serial.println(packet.localPort());
  Serial.print("data length: ");
  Serial.println(packet.length());
  Serial.print("data content: ");
  Serial.write(packet.data(), packet.length());
  Serial.println();
  // Send a reply back to the source of the packet
  packet.print("reply data");
  broadcastPort = packet.remotePort();  // Update the broadcast port to the remote port
}

void setup() {
  Serial.begin(115200);  // Start serial communication
  WiFi.begin(ssid, password);  // Initiate WiFi connection
  // Wait for WiFi to connect
  while (!WiFi.isConnected()) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected");
  Serial.print("IP Address:");
  Serial.println(WiFi.localIP());  // Print the assigned IP address

  // Start listening for UDP packets on the local port
  while (!udp.listen(localUdpPort)) {
  }
  // Set the callback function to be called when a packet is received
  udp.onPacket(onPacketCallBack);
}

void loop() {
  delay(5000);  // Wait for 5 seconds

  // Send a broadcast message to the previously received remote port
  udp.broadcastTo(broadcastData, broadcastPort); 
  // The following lines are commented out as they are not used in this example
  /*
  IPAddress broadcastAddr((~(uint32_t)WiFi.subnetMask())|((uint32_t)WiFi.localIP())); 
  udp.writeTo(broadcastData, sizeof(broadcastData), broadcastAddr, localUdpPort);
  */
}
