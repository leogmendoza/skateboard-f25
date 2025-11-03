#include <Arduino.h>
#include "BmsAPI.h"

// ESP32 example: use Serial2 (pins depend on your board; fine for now)
BmsAPI bms(Serial2, /*rs485_dir_pin=*/-1);

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("BmsAPI demo (FAKE MODE) starting...");
  bms.begin(9600);  // JBD default
}

void loop() {
  if (bms.update()) {
    const auto& s = bms.latest();
    Serial.print("V=");   Serial.print(s.pack_voltage_V, 3); Serial.print(" V  ");
    Serial.print("I=");   Serial.print(s.pack_current_A, 3); Serial.print(" A  ");
    Serial.print("SOC="); Serial.print(s.soc_percent);       Serial.print("%  ");
    Serial.print("CHG="); Serial.print(s.charge_fet_on ? "ON" : "off"); Serial.print("  ");
    Serial.print("DSG="); Serial.println(s.discharge_fet_on ? "ON" : "off");
  }
  delay(300);
}
