#pragma once 

#include <stdint.h>
#include <stdbool.h>

#define BUTTON_STATE_PRESSED 0  // Active-low
#define BUTTON_STATE_UNPRESSED 1
#define BUTTON_DEBOUNCE_DELAY_MS 50

typedef struct {
    volatile uint8_t *pin;  // PINx for reading pins
    uint8_t bit;  // x in PINx
    uint8_t prev_state;
    uint32_t prev_debounce_ms;
    bool pressed_flag;
} LightsButton;

/**
 * @brief Configure button pin as input and initialize debounce variables
 */
void lights_button_init(LightsButton *button, volatile uint8_t *pin, volatile uint8_t *ddr, volatile uint8_t *port, uint8_t bit);

/**
 * @brief Poll button state and handle debounce timing
 */
void lights_button_update(LightsButton *button, uint32_t current_time_ms);

/**
 * @brief Return true once per valid button press event
 */
bool lights_button_is_pressed(LightsButton *button);