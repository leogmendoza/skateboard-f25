#ifndef OLED_DISPLAY_API_H
#define OLED_DISPLAY_API_H

#include <Arduino.h>

void display_init();

void reset_OLED();

void speed_updateOLED(float speed_kmh);
void battery_updateOLED(float battery_percent);
void temp_updateOLED(float temp_celsius);
void warning_updateOLED(const char* warning); 

void display_refresh();

void update_display(float speed, float battery, float temp, const char* warning);

#endif