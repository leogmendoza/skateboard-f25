#pragma once 

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    volatile uint8_t *pin_reg;
    uint8_t bit;
    uint8_t prev_state;
    uint32_t prev_debounce_ms;
    bool pressed_flag;
} LightsButton;

/**
 * @brief Configure button pin as input and initialize debounce variables
 */
void lights_button_init(LightsButton *button, volatile uint8_t *ddr, volatile uint8_t *port, uint8_t bit);

/**
 * @brief Poll button state and handle debounce timing
 */
void lights_button_update(LightsButton *button, uint32_t current_time_ms);

/**
 * @brief Return true once per valid button press event
 */
bool lights_button_is_pressed(LightsButton *button);