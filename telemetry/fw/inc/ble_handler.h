#ifndef BLE_HANDLER_H
#define BLE_HANDLER_H
#include <NimBLEDevice.h>

void initBLE();
void notifyBLEData(NimBLECharacteristic* pCharacteristic, String test);
bool isClientConnected();

// whatever characteristics you want to include (battery life, speed, etc.)
extern NimBLECharacteristic* pCharacteristic1;
extern NimBLECharacteristic* pCharacteristic2;
extern NimBLECharacteristic* pCharacteristic3;
extern NimBLECharacteristic* pCharacteristic4;


#endif