#include <Arduino.h>
#include <NimBLEDevice.h>
#include "ble_handler.h"

#define DEVICE_NAME "ESP32 Telemetry"
#define SERVICE_1_UUID "668f0588-ed4e-4acc-806f-5e1c6a1a9742"
#define CHARACTERISTIC_1A_UUID "19d3b3a8-8a9b-4ad7-90e5-1da3deca02e4"
#define CHARACTERISTIC_1B_UUID "146821d3-189a-4434-bc6f-d6a1fabbd81b"
#define CHARACTERISTIC_1C_UUID "44db43fe-7333-44f6-bb8d-5c627ee419a8"
#define CHARACTERISTIC_1D_UUID "c3c64a51-8839-42a4-a8e3-6f0d80ed01e9"
NimBLECharacteristic *pCharacteristic1 = nullptr;
NimBLECharacteristic *pCharacteristic2 = nullptr;
NimBLECharacteristic *pCharacteristic3 = nullptr;
NimBLECharacteristic *pCharacteristic4 = nullptr;
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
    pCharacteristic->setValue("Read");
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

  // Characteristics **************************************************************************
  pCharacteristic1 = pService->createCharacteristic(
    CHARACTERISTIC_1A_UUID,
    NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY
  );
  pCharacteristic2 = pService->createCharacteristic(
    CHARACTERISTIC_1B_UUID,
    NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY
  );
  pCharacteristic3 = pService->createCharacteristic(
    CHARACTERISTIC_1C_UUID,
    NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY
  );
  pCharacteristic4 = pService->createCharacteristic(
    CHARACTERISTIC_1D_UUID,
    NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY
  );
  // Add more Characteristics if you want to send more data to phone

  // Descriptors for each Characteristic *********************************************************************************
  NimBLEDescriptor *pDescriptor1 = pCharacteristic1->createDescriptor( // Change descriptor name to whatever you want
    "2901",
    NIMBLE_PROPERTY::READ
  );
  pDescriptor1->setValue("Speed (km/h)");
  //*********************************************************************************** */
  NimBLEDescriptor *pDescriptor2 = pCharacteristic2->createDescriptor( // Change descriptor name to whatever you want
    "2901",
    NIMBLE_PROPERTY::READ
  );
  pDescriptor2->setValue("Distance (km)");
  //************************************************************************************ */
  NimBLEDescriptor *pDescriptor3 = pCharacteristic3->createDescriptor( // Change descriptor name to whatever you want
    "2901",
    NIMBLE_PROPERTY::READ
  );
  pDescriptor3->setValue("Battery (%)");
  //************************************************************************************ */
  NimBLEDescriptor *pDescriptor4 = pCharacteristic4->createDescriptor( // Change descriptor name to whatever you want
    "2901",
    NIMBLE_PROPERTY::READ
  );
  pDescriptor4->setValue("Pack Voltage (V)");

  pCharacteristic1->setCallbacks(new MyCharacteristicCallbacks());
  pCharacteristic2->setCallbacks(new MyCharacteristicCallbacks());
  pCharacteristic3->setCallbacks(new MyCharacteristicCallbacks());
  pCharacteristic4->setCallbacks(new MyCharacteristicCallbacks());

  pService->start();

  // Start Advertising with the service UUID
  NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
  pAdvertising->setScanResponse(true);
  pAdvertising->start();

}

void notifyBLEData(NimBLECharacteristic* pCharacteristic, String test){ // Change String test into whatever data you want to pass
  if (pCharacteristic != nullptr && pCharacteristic->getSubscribedCount() > 0) {
    pCharacteristic->setValue(test);
    pCharacteristic->notify();
  }
}

bool isClientConnected() {
  return pServer != nullptr && pServer->getConnectedCount() > 0;
}
// put     notifyBLEData(pCharacteristic1, "one");
//         notifyBLEData(pCharacteristic2, "two");
//         notifyBLEData(pCharacteristic3, "three");
//         notifyBLEData(pCharacteristic4, "four");
// into main function
// or using a different argument than "one", etc, if you would like
