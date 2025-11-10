#pragma once

#include "config.h"
#include "fsm.h"
#include "button.h"
#include "led.h"

#define NUM_TAIL_LEDS 4

typedef struct {
    LightsFsm fsm;
    LightsButton button;
    LightsLed leds[NUM_TAIL_LEDS];
} TailLight;

/**
 * @brief Initialize taillight FSM, LEDs, button, and accelerometer
 */
void tail_light_init(TailLight *tail);

/**
 * @brief Handle taillight input and update LED brightness or brake mode
 */
void tail_light_update(TailLight *tail);