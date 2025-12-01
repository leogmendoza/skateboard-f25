#pragma once

#include <stdint.h>

#include "fsm.h"
#include "utils.h"

#define LIGHTS_LED_DEFAULT_BRIGHTNESS 255

typedef struct {
    PwmChannel pwm_channel;
    uint8_t brightness;
} LightsLed;

/**
 * @brief Configure LED PWM channel and set default brightness
 */
void lights_led_init(LightsLed *led, PwmChannel pwm_channel);

/**
 * @brief Store new brightness value for the LED
 */
void lights_led_set_brightness(LightsLed *led, uint8_t brightness);

/**
 * @brief Apply stored brightness to the hardware PWM output
 */
void lights_led_update(LightsLed *led);