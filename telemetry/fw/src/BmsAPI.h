#pragma once
#include <Arduino.h>

// ===== Fake mode ON: we don't have the BMS yet =====
#ifndef BMS_FAKE_MODE
#define BMS_FAKE_MODE 1
#endif

// A tiny "snapshot" of data the rest of the code can read
struct BmsSnapshot {
  uint32_t ms = 0;            // last update time
  float    pack_voltage_V = NAN;
  float    pack_current_A = NAN; // +charge / -discharge
  uint8_t  soc_percent   = 255;  // 0..100
  uint16_t cycle_count   = 0;
  bool     charge_fet_on = false;
  bool     discharge_fet_on = false;
};

class BmsAPI {
public:
  explicit BmsAPI(HardwareSerial& port, int rs485_dir_pin = -1);
  bool begin(uint32_t uart_baud = 9600); // start serial
  bool update();                         // refresh internal values

  // simple getters
  const BmsSnapshot& latest() const { return _snap; }
  float   voltage()   const { return _snap.pack_voltage_V; }
  float   current()   const { return _snap.pack_current_A; }
  uint8_t soc()       const { return _snap.soc_percent; }
  bool    chargeOn()  const { return _snap.charge_fet_on; }
  bool    dischargeOn() const { return _snap.discharge_fet_on; }
  uint16_t cycles()   const { return _snap.cycle_count; }

private:
  HardwareSerial* _serial;
  int _dir_pin;
  BmsSnapshot _snap{};

#if BMS_FAKE_MODE
  uint32_t _t0 = 0;  // for fake data timing
#else
  // real library handles go here later
  // #include <jbdbms.h>
  // JbdBms* _driver = nullptr;
  // JbdBms::Status_t _raw{};
#endif
};
