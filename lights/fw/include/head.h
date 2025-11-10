#pragma once

#include "config.h"
#include "fsm.h"
#include "button.h"
#include "led.h"

#define NUM_HEAD_LEDS 4

typedef struct {
    LightsFsm fsm;
    LightsButton button;
    LightsLed leds[NUM_HEAD_LEDS];
} HeadLight;

/**
 * @brief Initialize headlight FSM, LEDs, and button
 */
void head_light_init(HeadLight *head);

/**
 * @brief Handle headlight button input and apply LED brightness per mode
 */
void head_light_update(HeadLight *head);