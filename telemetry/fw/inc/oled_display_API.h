#pragma once

// Basic OLED control
void display_init();
void reset_OLED();
void display_refresh();

// Lower-level update helpers
void speed_updateOLED(float speed_kmh);
void battery_updateOLED(float battery_percent);
void temp_updateOLED(float temp_celsius);
void warning_updateOLED(const char* warning);

// Original composite updater (speed, battery, temp, warning)
void update_display(float speed, float battery, float temp, const char* warning);

// NEW: minimal API for speed + distance (with battery %)
void update_speed_distance(float speedKmh, float distanceKm, float batteryPercent);
