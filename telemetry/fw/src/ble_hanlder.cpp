#include "ble_handler.h"
#include <Arduino.h>
#include <NimBLEDevice.h>

#define DEVICE_NAME "hihihi"
#define SERVICE_1_UUID "668f0588-ed4e-4acc-806f-5e1c6a1a9742"
#define CHARACTERISTIC_1A_UUID "19d3b3a8-8a9b-4ad7-90e5-1da3deca02e4"
NimBLECharacteristic *pCharacteristic = nullptr;
NimBLEServer *pServer = nullptr;

//** Callbacks *******/
class MyServerCallbacks : public NimBLEServerCallbacks {

  void onConnect(NimBLEServer *pServer){
    digitalWrite(2, HIGH);
    Serial.println("Client Connected");
  }

  void onDisconnect(NimBLEServer *pServer) {
    digitalWrite(2, LOW);
    Serial.println("Client Disconnected");
    NimBLEDevice::startAdvertising();
  }
};

class MyCharacteristicCallbacks : public NimBLECharacteristicCallbacks {

  void onRead(NimBLECharacteristic *pCharacteristic){
    uint32_t currentMillis = millis() / 1000;
    pCharacteristic->setValue((String("67 ") + currentMillis).c_str());
  }
};

void initBLE() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial.println("ESP32 BLE Server setup beginning...");

  // Pin modes
  pinMode(2, OUTPUT);

  // Initialize Device
  NimBLEDevice::init(DEVICE_NAME);

  // Create server
  pServer = NimBLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Services
  NimBLEService *pService = pServer->createService(SERVICE_1_UUID);

  // Characteristics
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_1A_UUID,
    NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY
  );

  pCharacteristic->setCallbacks(new MyCharacteristicCallbacks());

  pService->start();

  // Start Advertising WITH the service UUID
  NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
  pAdvertising->setScanResponse(true);
  pAdvertising->start();

}

void notifyBLEData(){
  // push data to phoneuint32_t currentMillis = millis() / 1000
  if (pCharacteristic != nullptr && pCharacteristic->getSubscribedCount() > 0) {
    uint32_t currentMillis = millis() / 1000;
    String value = (String("67 ") + currentMillis).c_str();
    pCharacteristic->setValue((uint8_t*)value.c_str(), value.length());
    pCharacteristic->notify();
  }
}