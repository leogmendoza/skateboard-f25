#pragma once

#include <stdint.h>

#include "fsm.h"
#include "utils.h"

#define LIGHTS_LED_BRIGHTNESS_OFF 0
#define LIGHTS_LED_BRIGHTNESS_DEFAULT 255

typedef struct {
    volatile uint8_t *ddr;  // DDRx for writing to pins
    volatile uint8_t *port;  // PORTx for readin pins
    uint8_t bit;  // x in PINx
    PwmChannel pwm_channel;
    uint8_t brightness;
} LightsLed;

/**
 * @brief Configure LED PWM channel and set default brightness
 */
void lights_led_init(LightsLed *led, volatile uint8_t *ddr, volatile uint8_t *port, uint8_t bit, PwmChannel pwm_channel);

/**
 * @brief Store new brightness value for the LED
 */
void lights_led_set_brightness(LightsLed *led, uint8_t brightness);

/**
 * @brief Apply stored brightness to the hardware PWM output
 */
void lights_led_update(LightsLed *led);