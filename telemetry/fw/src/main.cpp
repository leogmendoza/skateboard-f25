#include <Arduino.h>
#include "BmsAPI.h"

#define RS485_DIR_PIN 22

BmsAPI bmsAPI(Serial2); 
void setup() {
    Serial.begin(115200);
    if (!bmsAPI.begin(RS485_DIR_PIN, 9600)) {
        Serial.println("Failed to initialize BMS communication!");
    }
}

void loop() {
    bmsAPI.update();

    Serial.print("Voltage: ");  
    Serial.print(bmsAPI.getVoltage());  
    Serial.println(" V");
    Serial.print("Current: ");  
    Serial.print(bmsAPI.getCurrent());  
    Serial.println(" A");
    Serial.print("Charge: ");   
    Serial.print(bmsAPI.getChargePercentage()); 
    Serial.println("%");

    Serial.print("Average Temp: ");
    Serial.print(bmsAPI.getAverageTemperature());
    Serial.println(" °C");

    delay(1000);
}
