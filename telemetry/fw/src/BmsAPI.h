#pragma once
#include <jbdbms.h>

class BmsAPI {
public:
    explicit BmsAPI(HardwareSerial &port);
    bool begin(int dirPin, unsigned long baudRate = 9600);
    void update();

    float getVoltage() const;
    float getCurrent() const;
    float getChargePercentage() const;
    float getAverageTemperature() const;

private:
    JbdBms bms;  
    JbdBms::Status_t status;
};
