#include "vescParser.hpp"
#include <HardwareSerial.h>  // Required for Serial2

// Constructor - initializes the dummy data flag
VescParser::VescParser() : useDummyData(true) {
  data.rpm = 0.0f;
  data.inpVoltage = 0.0f;
  data.ampHours = 0.0f;
  data.tachometerAbs = 0;
}

void VescParser::initialize() {
  Serial.begin(115200);
  while (!Serial) {
    ;  // wait for USB serial
  }

  Serial.println("Initializing VESC UART simulation...");

  if (!useDummyData) {
    // Initialize Serial2 if we're using a real VESC
    Serial2.begin(19200, SERIAL_8N1, 16, 17);
    UART.setSerialPort(&Serial2);
  }
}

bool VescParser::getData() {
  if (useDummyData) {
    // Simulated data for testing
    data.rpm = 4200.0;
    data.inpVoltage = 47.8;
    data.ampHours = 1.25;
    data.tachometerAbs = 5630;
    return true;
  } else {
    // Read the VESC data from the real VESC via UART
    return UART.getVescValues();
  }
}

float VescParser::getRPM() const {
  return data.rpm;
}

float VescParser::getInputVoltage() const {
  return data.inpVoltage;
}

float VescParser::getAmpHours() const {
  return data.ampHours;
}

int32_t VescParser::getTachometerAbs() const {
  return data.tachometerAbs;
}
 