#include "BmsAPI.h"
#include <math.h>

BmsAPI::BmsAPI(HardwareSerial& port, int rs485_dir_pin)
: _serial(&port), _dir_pin(rs485_dir_pin) {}

bool BmsAPI::begin(uint32_t uart_baud) {
  _serial->begin(uart_baud
#if defined(SERIAL_8N1)
  , SERIAL_8N1
#endif
  );
#if BMS_FAKE_MODE
  _t0 = millis();
  return true;
#else
  _driver = new JbdBms(*_serial);
  _driver->begin(_dir_pin);   // -1 for plain UART; DE/RE pin if RS485
  return true;
#endif
}

bool BmsAPI::update() {
#if BMS_FAKE_MODE
  // ---- FAKE MODE: generate pretend-but-believable values ----
  const uint32_t now = millis();
  const float t = (now - _t0) / 1000.0f;

  _snap.ms = now;
  _snap.pack_voltage_V   = 44.0f + 0.25f * sinf(t * 0.4f);   // ~44.0–44.25 V
  _snap.pack_current_A   = -3.0f + 1.5f * sinf(t * 0.7f);    // discharge-ish
  _snap.soc_percent      = (uint8_t) constrain(80 + 10 * sinf(t * 0.05f), 0, 100);
  _snap.cycle_count      = 123;
  _snap.charge_fet_on    = (_snap.pack_current_A > -0.2f);
  _snap.discharge_fet_on = (_snap.pack_current_A <  0.2f);
  return true;
#else
  // ---- REAL MODE: pull real values from Joba_JbdBms ----
  JbdBms::Status_t s;
  if (!_driver->getStatus(s)) return false;   // no fresh frame this call

  _raw = s;  // optional: keep raw for debugging

  // Units from jbdbms.h:
  //   voltage: 10 mV units  -> V = *0.01
  //   current: 10 mA units  -> A = *0.01 (signed: +charge, -discharge)
  //   currentCapacity: percent (0..100)
  //   cycles: count
  //   mosfetStatus: enum { NONE, CHARGE, DISCHARGE, BOTH }
  _snap.ms             = millis();
  _snap.pack_voltage_V = s.voltage * 0.01f;                       // 10 mV → V
  _snap.pack_current_A = static_cast<int16_t>(s.current) * 0.01f; // 10 mA → A
  _snap.soc_percent    = s.currentCapacity;                       // %
  _snap.cycle_count    = s.cycles;

  JbdBms::mosfet_t m   = static_cast<JbdBms::mosfet_t>(s.mosfetStatus);
  _snap.charge_fet_on    = (m == JbdBms::MOSFET_CHARGE    || m == JbdBms::MOSFET_BOTH);
  _snap.discharge_fet_on = (m == JbdBms::MOSFET_DISCHARGE || m == JbdBms::MOSFET_BOTH);
  return true;
#endif
}bool BmsAPI::readCells(BmsCells &out) {
#if BMS_FAKE_MODE
  // Fake: 12 cells around 3.70 V with tiny variation
  out.count = 12;
  for (uint8_t i = 0; i < out.count; ++i) {
    out.millivolts[i] = 3700 + (i % 3) * 5; // 3700, 3705, 3710...
  }
  return true;
#else
  // Real: issue getCells() and use the last known cell count from _raw (set by update())
  JbdBms::Cells_t cells{};
  if (!_driver->getCells(cells)) return false;

  uint8_t n = _raw.cells;                 // how many cells the BMS reported in last status
  if (n == 0 || n > BMS_MAX_CELLS) n = BMS_MAX_CELLS;

  out.count = n;
  for (uint8_t i = 0; i < n; ++i) {
    out.millivolts[i] = cells.voltages[i]; // library gives mV
  }
  return true;
#endif
}

bool BmsAPI::readTemps(BmsTemps &out) {
#if BMS_FAKE_MODE
  // Fake: 2 NTCs ~25–26 °C
  out.count = 2;
  out.celsius[0] = 25.0f;
  out.celsius[1] = 26.0f;
  return true;
#else
  // Real: temps are included in the last Status_t we stored in _raw during update()
  uint8_t n = _raw.ntcs;                  // number of NTCs the BMS reports
  if (n == 0 || n > BMS_MAX_NTCS) n = BMS_MAX_NTCS;

  out.count = n;
  for (uint8_t i = 0; i < n; ++i) {
    // library helper: deciCelsius() = 0.1 °C; convert to °C
    const int16_t dC = JbdBms::deciCelsius(_raw.temperatures[i]);
    out.celsius[i] = dC / 10.0f;
  }
  return true;
#endif
}

