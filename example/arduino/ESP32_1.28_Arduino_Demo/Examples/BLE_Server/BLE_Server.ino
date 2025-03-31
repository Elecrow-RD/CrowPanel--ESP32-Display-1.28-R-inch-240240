#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"  // UUID for the service
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"  // UUID for the characteristic

BLECharacteristic *pCharacteristic;  // Pointer to the BLE characteristic

void setup() {
  Serial.begin(115200);  // Initialize serial communication at a baud rate of 115200
  BLEDevice::init("ESP32C3_BLE_Server");  // Initialize the BLE device with a name
  BLEServer *pServer = BLEDevice::createServer();  // Create a BLE server
  BLEService *pService = pServer->createService(SERVICE_UUID);  // Create a service with the specified UUID

  // Create a characteristic with read and notify properties
  pCharacteristic = pService->createCharacteristic(
                       CHARACTERISTIC_UUID,
                       BLECharacteristic::PROPERTY_READ |
                       BLECharacteristic::PROPERTY_NOTIFY
                     );
                     
  pCharacteristic->setValue("Hello World");  // Set the initial value of the characteristic
  pService->start();  // Start the service
  pServer->getAdvertising()->start();  // Start advertising the BLE device
}

void loop() {
  // This loop function does nothing in this example as the BLE server does not require
  // continuous processing in the loop once it's set up and running.
}
