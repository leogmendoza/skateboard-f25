#pragma once

#include "config.h"
#include "fsm.h"
#include "button.h"
#include "led.h"

#define NUM_TAIL_LEDS 4

#define FLASH_STEP_MS 500

#define TAIL_BRIGHTNESS_BRAKING 255
#define TAIL_BRIGHTNESS_DEFAULT 150
#define TAIL_BRIGHTNESS_STATIONARY 50

typedef struct {
    LightsFsm fsm;
    LightsButton button;
    LightsLed leds[NUM_TAIL_LEDS];

    uint8_t startup_cycle;
} TailLight;

extern volatile uint8_t * const tail_ddr_sequence[];
extern volatile uint8_t * const tail_port_sequence[];
extern const uint8_t tail_bit_sequence[];

/**
 * @brief Initialize taillight FSM, LEDs, button, and accelerometer
 */
void tail_light_init(TailLight *tail);

/**
 * @brief Handle taillight input and update LED brightness or brake mode
 */
void tail_light_update(TailLight *tail);