#include "oled_display_API.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64
#define OLED_RESET      -1
#define OLED_ADDRESS    0x3C

#define SPEED_X_POS     0
#define SPEED_Y_POS     0
#define BATT_X_POS      0
#define BATT_Y_POS      20
#define TEMP_X_POS      0
#define TEMP_Y_POS      32
#define WARN_X_POS      0
#define WARN_Y_POS      44

#define BATTERY_THRESHOLD       20.0
#define TEMPERATURE_THRESHOLD   60.0

#define MAX_WARNING_LEN 24

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

static float speed_default   = 0.0f;
static float battery_default = 0.0f;
static float temp_default    = 0.0f;
static char  warning_default[MAX_WARNING_LEN] = "";

void display_init() {
  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    // If init fails, hang – there is no display
    for (;;) { }
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("connecting..");
  display.display();
  delay(1000);

  reset_OLED();
  display.display();
}

void reset_OLED() {
  display.clearDisplay();
  speed_default   = 0.0f;
  battery_default = 0.0f;
  temp_default    = 0.0f;
  warning_default[0] = '\0';
}

void speed_updateOLED(float speed_kmh) {
  speed_default = speed_kmh;
}

void battery_updateOLED(float battery_percent) {
  battery_default = battery_percent;
}

void temp_updateOLED(float temp_celsius) {
  temp_default = temp_celsius;
}

void warning_updateOLED(const char* warning) {
  if (warning == nullptr) {
    warning_default[0] = '\0';
  } else {
    strncpy(warning_default, warning, MAX_WARNING_LEN - 1);
    warning_default[MAX_WARNING_LEN - 1] = '\0';
  }
}

void display_refresh() {
  display.clearDisplay();

  // Speed line
  display.setTextSize(2);
  display.setCursor(SPEED_X_POS, SPEED_Y_POS);
  display.print(speed_default, 0);
  display.print(" KM/H");

  // Battery line
  display.setTextSize(1);
  display.setCursor(BATT_X_POS, BATT_Y_POS);
  display.print("Battery: ");
  display.print(battery_default, 0);
  display.print("%");

  // Temp line
  display.setCursor(TEMP_X_POS, TEMP_Y_POS);
  display.print("Temp: ");
  display.print(temp_default, 0);
  display.print("C");

  // Warning / status
  display.setCursor(WARN_X_POS, WARN_Y_POS);

  const char* final_warning = nullptr;

  if (battery_default <= BATTERY_THRESHOLD) {
    final_warning = "LOW BATTERY!";
  } else if (temp_default >= TEMPERATURE_THRESHOLD) {
    final_warning = "HIGH TEMP!";
  } else if (warning_default[0] != '\0') {
    final_warning = warning_default;
  }

  if (final_warning != nullptr) {
    display.setTextSize(1);
    display.print("! ");
    display.print(final_warning);
  } else {
    display.print("Status: Normal");
  }

  display.display();
}

void update_display(float speed, float battery, float temp, const char* warning) {
  speed_updateOLED(speed);
  battery_updateOLED(battery);
  temp_updateOLED(temp);
  warning_updateOLED(warning);
  display_refresh();
}

// ===== NEW: minimal speed + distance display =====
// Clean "rider view" for speed, distance, and battery %.
void update_speed_distance(float speedKmh, float distanceKm, float batteryPercent) {
  display.clearDisplay();

  // Big speed at the top
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print(speedKmh, 1);
  display.print(" km/h");

  // Distance below
  display.setTextSize(1);
  display.setCursor(0, 28);
  display.print("Dist: ");
  display.print(distanceKm, 2);
  display.print(" km");

  // Battery percent
  display.setCursor(0, 44);
  display.print("Battery: ");
  display.print(batteryPercent, 0);
  display.print("%");

  display.display();
}
