#include "BmsAPI.h"
#include <math.h>

BmsAPI::BmsAPI(HardwareSerial& port, int rs485_dir_pin)
: _serial(&port), _dir_pin(rs485_dir_pin) {}

bool BmsAPI::begin(uint32_t uart_baud) {
  // bring up the UART you passed (e.g., Serial2)
  _serial->begin(uart_baud
#if defined(SERIAL_8N1)
  , SERIAL_8N1
#endif
  );
#if BMS_FAKE_MODE
  _t0 = millis();
  return true;
#else
  // real library init goes here later
  // _driver = new JbdBms(*_serial);
  // _driver->begin(_dir_pin);  // -1 for plain UART
  return true;
#endif
}

bool BmsAPI::update() {
#if BMS_FAKE_MODE
  // ---- generate pretend-but-believable values ----
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
  // real polling/parsing goes here later
  return false;
#endif
}
