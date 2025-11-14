#pragma once
#include <Arduino.h>

// default to FAKE unless env turns it off
#ifndef BMS_FAKE_MODE
#define BMS_FAKE_MODE 1
#endif

#if !BMS_FAKE_MODE
  #include <jbdbms.h>  // Joba_JbdBms
#endif

struct BmsSnapshot {
  uint32_t ms = 0;
  float    pack_voltage_V = NAN;
  float    pack_current_A = NAN; // +charge / -discharge
  uint8_t  soc_percent   = 255;  // 0..100
  uint16_t cycle_count   = 0;
  bool     charge_fet_on = false;
  bool     discharge_fet_on = false;
};
// Max supported by the JBD lib header
static constexpr uint8_t BMS_MAX_CELLS = 32;
static constexpr uint8_t BMS_MAX_NTCS  = 8;

// One-shot snapshots fetched on demand
struct BmsCells {
  uint8_t  count = 0;                 // how many cells are valid
  uint16_t millivolts[BMS_MAX_CELLS]; // mV per cell
};

struct BmsTemps {
  uint8_t count = 0;                  // how many NTCs are valid
  float   celsius[BMS_MAX_NTCS];      // °C per NTC
};

class BmsAPI {
public:
  explicit BmsAPI(HardwareSerial& port, int rs485_dir_pin = -1);
  bool begin(uint32_t uart_baud = 9600);
  bool update();

  const BmsSnapshot& latest() const { return _snap; }
  float   voltage()   const { return _snap.pack_voltage_V; }
  float   current()   const { return _snap.pack_current_A; }
  uint8_t soc()       const { return _snap.soc_percent; }
  bool    chargeOn()  const { return _snap.charge_fet_on; }
  bool    dischargeOn() const { return _snap.discharge_fet_on; }
  uint16_t cycles()   const { return _snap.cycle_count; }
  // NEW: extra data the main loop may want
  bool readCells(BmsCells &out);  // fills per-cell mV (returns true on success)
  bool readTemps(BmsTemps &out);  // fills °C from last status frame

private:
  HardwareSerial* _serial;
  int _dir_pin;
  BmsSnapshot _snap{};

#if BMS_FAKE_MODE
  uint32_t _t0 = 0;
#else
  JbdBms* _driver = nullptr;
  JbdBms::Status_t _raw{};
#endif
};
