#include "BmsAPI.h"
#include <Arduino.h>

BmsAPI::BmsAPI(HardwareSerial &port)
    : bms(port)
{}

bool BmsAPI::begin(int dirPin, unsigned long baudRate) {
    bms.begin(dirPin);
    return true;
}

void BmsAPI::update() {
    bms.getStatus(status);
}

float BmsAPI::getVoltage() const { return status.voltage / 100.0f; }
float BmsAPI::getCurrent() const { return status.current / 100.0f; }
float BmsAPI::getChargePercentage() const { return status.currentCapacity; }
float BmsAPI::getAverageTemperature() const {
    if (status.ntcs == 0) return NAN;

    float sum = 0.0f;
    for (int i = 0; i < status.ntcs; ++i) {
        int16_t deciC = JbdBms::deciCelsius(status.temperatures[i]);
        sum += deciC / 10.0f;
    }
    return sum / status.ntcs;
}
