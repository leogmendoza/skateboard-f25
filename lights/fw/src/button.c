#include "button.h"

#include <stdbool.h>

#include "utils.h"

void lights_button_init(LightsButton *button, volatile uint8_t *pin, volatile uint8_t *ddr, volatile uint8_t *port, uint8_t bit) {
    utils_gpio_set_input(ddr, port, bit, true);

    button->pin = pin;
    button->bit = bit;
    button->prev_state = BUTTON_STATE_UNPRESSED;  // Using internal pull-up
    button->prev_debounce_ms = utils_uptime_ms();
    button->pressed_flag = false;
}

void lights_button_update(LightsButton *button, uint32_t current_time_ms) {
    uint8_t curr_state = utils_gpio_read(button->pin, button->bit);

    // State is considered stable, so "reset" debounce timer
    if (curr_state == button->prev_state) {
        button->prev_debounce_ms = current_time_ms;
        return;
    }

    // State changed, but not long enough, so start debounce timer!
    if ( (current_time_ms - button->prev_debounce_ms) < BUTTON_DEBOUNCE_DELAY_MS ) {
        return;
    }

    // State has stabilized, so accept new state fr fr
    button->prev_state = curr_state;
    button->prev_debounce_ms = current_time_ms;

    // Valid falling edge triggers button press event
    if (curr_state == BUTTON_STATE_PRESSED) {
        button->pressed_flag = true;
    }
}

bool lights_button_is_pressed(LightsButton *button) {
    bool temp = button->pressed_flag;
    button->pressed_flag = false;  // Clear since it was just checked

    return temp;
}