#pragma once

#include <stdint.h>

#include "fsm.h"

typedef struct {
    uint8_t pwm_channel;
    uint8_t brightness;
} LightsLed;

void lights_led_init(LightsLed *led, uint8_t pwm_channel);
void lights_led_set_brightness(LightsLed *led, uint8_t brightness);
void lights_led_update(LightsLed *led);