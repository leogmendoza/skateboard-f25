#include "tail.h"

#include "utils.h"
#include "config.h"
#include "fsm.h"
#include "button.h"

static const volatile uint8_t *tail_ddr_sequence[] = {
    &DDRD,
    &DDRD,
    &DDRB,
    &DDRD,
};

static const volatile uint8_t *tail_port_sequence[] = {
    &PORTD,
    &PORTD,
    &PORTB,
    &PORTD,
};

static const uint8_t tail_bit_sequence[] = {
    PD6,
    PD5,
    PB3,
    PD3,
};

static const LightsState tail_state_sequence[] = {
    LIGHTS_STATE_STARTUP,
    LIGHTS_STATE_SOLID,
    LIGHTS_STATE_FLASH,
    LIGHTS_STATE_OFF,
};

void tail_light_init(TailLight *tail) {
    lights_fsm_init(&tail->fsm, tail_state_sequence, NUM_STATES);
    lights_button_init(&tail->button, TAIL_BUTTON_PIN, TAIL_BUTTON_DDR, TAIL_BUTTON_PORT, TAIL_BUTTON_BIT);

    for (uint8_t i = 0; i < NUM_TAIL_LEDS; i++) {
        lights_led_init(&tail->leds[i], tail_ddr_sequence[i], tail_port_sequence[i], tail_bit_sequence[i], i);  // Index maps to PwmChannel enum
    }

    tail->startup_cycle = 0;
}

void tail_light_update(TailLight *tail) {
    uint32_t curr_time_ms = utils_uptime_ms();

    switch (lights_fsm_get_state(&tail->fsm)) {
        case LIGHTS_STATE_STARTUP: {
            uint32_t delta_time_ms = (curr_time_ms - tail->fsm.last_transition_ms);

            uint8_t step = delta_time_ms / STARTUP_STEP_TIME_MS;

            if (step >= NUM_TAIL_LEDS) {
                tail->startup_cycle++;
                tail->fsm.last_transition_ms = curr_time_ms;   // Reset timer for next cycle
                step = 0;
            }

            for (uint8_t i = 0; i < NUM_TAIL_LEDS; i++) {
                if (i == step) {
                    lights_led_set_brightness(&tail->leds[i], LIGHTS_LED_BRIGHTNESS_DEFAULT);
                } else {
                    lights_led_set_brightness(&tail->leds[i], LIGHTS_LED_BRIGHTNESS_OFF);
                }
            }

            // Automatically transition to next state after repeating many cycles
            if (tail->startup_cycle >= STARTUP_TOTAL_CYCLES) {
                lights_fsm_update(&tail->fsm, LIGHTS_EVENT_STARTUP_COMPLETE);
            }

            break;
        }

        case LIGHTS_STATE_SOLID: {
            for (uint8_t i = 0; i < NUM_TAIL_LEDS; i++) {
                // TODO: ACCEL STUFF

                lights_led_set_brightness(&tail->leds[i], LIGHTS_LED_BRIGHTNESS_DEFAULT);
            }
            break;
        }

        case LIGHTS_STATE_FLASH: {
            uint32_t delta_time_ms = (curr_time_ms - tail->fsm.last_transition_ms);

            if (delta_time_ms >= FLASH_STEP_MS) {
                for (uint8_t i = 0; i < NUM_TAIL_LEDS; i++) {
                    if (tail->leds[i].brightness > 0) {
                        lights_led_set_brightness(&tail->leds[i], LIGHTS_LED_BRIGHTNESS_OFF);
                    } else {
                        lights_led_set_brightness(&tail->leds[i], LIGHTS_LED_BRIGHTNESS_DEFAULT);
                    }
                }

                tail->fsm.last_transition_ms = curr_time_ms;   // Reset timer for next cycle
            }
            break;
        }

        case LIGHTS_STATE_OFF: {
            for (uint8_t i = 0; i < NUM_TAIL_LEDS; i++) {
                lights_led_set_brightness(&tail->leds[i], LIGHTS_LED_BRIGHTNESS_OFF);
            }
            break;
        }
    }

    for (uint8_t i = 0; i < NUM_TAIL_LEDS; i++) {
        lights_led_update(&tail->leds[i]);
    }

    lights_button_update(&tail->button, curr_time_ms);

    if (lights_button_is_pressed(&tail->button)) {
        lights_fsm_update(&tail->fsm, LIGHTS_EVENT_BUTTON_PRESS);
    }
}