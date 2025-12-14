#pragma once

#include "config.h"
#include "fsm.h"
#include "button.h"
#include "led.h"

#define NUM_TAIL_LEDS 4

#define FLASH_STEP_MS 500

typedef struct {
    LightsFsm fsm;
    LightsButton button;
    LightsLed leds[NUM_TAIL_LEDS];

    uint8_t startup_cycle;
} TailLight;

/**
 * @brief Initialize taillight FSM, LEDs, button, and accelerometer
 */
void tail_light_init(TailLight *tail);

/**
 * @brief Handle taillight input and update LED brightness or brake mode
 */
void tail_light_update(TailLight *tail);