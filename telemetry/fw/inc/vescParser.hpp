#ifndef VESC_PARSER_HPP
#define VESC_PARSER_HPP

#include <VescUart.h>

class VescParser {
public:
  // Constructor
  VescParser();

  // Public method to initialize the VESC UART
  void initialize();

  // Public method to read VESC data
  bool getData();

  // Getter methods for the data
  float getRPM() const;
  float getInputVoltage() const;
  float getAmpHours() const;
  int32_t getTachometerAbs() const;

private:
  // UART object to interact with VESC
  VescUart UART;

  // Data structure to hold VESC data
  struct VescData {
    float rpm;
    float inpVoltage;
    float ampHours;
    int32_t tachometerAbs;
  } data;

  // Flag to simulate data (for testing without a real VESC)
  bool useDummyData;
};

#endif  // VESC_PARSER_HPP
