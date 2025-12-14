#include "head.h"

#include "utils.h"
#include "config.h"
#include "fsm.h"
#include "button.h"

static const uint8_t head_bit_sequence[] = {
    PC3,
    PC2,
    PC1,
    PC0,
};

static const LightsState head_state_sequence[] = {
    LIGHTS_STATE_STARTUP,
    LIGHTS_STATE_SOLID,
    LIGHTS_STATE_STROBE,
    LIGHTS_STATE_OFF,
};

static const uint16_t head_strobe_pattern_timing_ms[] = {
    20,  // On
    20,  // Off
    20,  // On
    20,  // Off
    20,  // On
    120  // Rotund Off
};
#define STROBE_PATTERN_TIMING_LEN (sizeof(head_strobe_pattern_timing_ms) / sizeof(head_strobe_pattern_timing_ms[0]))

void head_light_init(HeadLight *head) {
    lights_fsm_init(&head->fsm, head_state_sequence, NUM_STATES);
    lights_button_init(&head->button, HEAD_BUTTON_PIN, HEAD_BUTTON_DDR, HEAD_BUTTON_PORT, HEAD_BUTTON_BIT);

    for (uint8_t i = 0; i < NUM_HEAD_LEDS; i++) {
        lights_led_init(&head->leds[i], HEAD_LEDS_DDR, HEAD_LEDS_PORT, head_bit_sequence[i], NO_PWM_CH);
    }

    head->startup_cycle = 0;
    head->strobe_step = 0;
}

void head_light_update(HeadLight *head) {
    switch (lights_fsm_get_state(&head->fsm)) {
        case LIGHTS_STATE_STARTUP: {
            uint32_t curr_time_ms = utils_uptime_ms();
            uint32_t delta_time_ms = (curr_time_ms - head->fsm.last_transition_ms);

            uint8_t step = delta_time_ms / STARTUP_STEP_TIME_MS;

            if (step >= NUM_HEAD_LEDS) {
                head->startup_cycle++;
                head->fsm.last_transition_ms = curr_time_ms;   // Reset timer for next cycle
                step = 0;
            }

            for (uint8_t i = 0; i < NUM_HEAD_LEDS; i++) {
                if (i == step) {
                    lights_led_set_brightness(&head->leds[i], LIGHTS_LED_BRIGHTNESS_DEFAULT);
                } else {
                    lights_led_set_brightness(&head->leds[i], LIGHTS_LED_BRIGHTNESS_OFF);
                }
            }

            // Automatically transition to next state after repeating many cycles
            if (head->startup_cycle >= STARTUP_TOTAL_CYCLES) {
                lights_fsm_update(&head->fsm, LIGHTS_EVENT_STARTUP_COMPLETE);
            }

            break;
        }

        case LIGHTS_STATE_SOLID: {
            for (uint8_t i = 0; i < NUM_HEAD_LEDS; i++) {
                lights_led_set_brightness(&head->leds[i], LIGHTS_LED_BRIGHTNESS_DEFAULT);
            }
            break;
        }

        case LIGHTS_STATE_STROBE: {
            uint32_t curr_time_ms = utils_uptime_ms();
            uint32_t delta_time_ms = curr_time_ms - head->fsm.last_transition_ms;

            if (delta_time_ms >= head_strobe_pattern_timing_ms[head->strobe_step]) {
                bool led_on = (head->strobe_step % 2) == 0;

                for (uint8_t i = 0; i < NUM_HEAD_LEDS; i++) {
                    if (led_on) {
                        lights_led_set_brightness(&head->leds[i],LIGHTS_LED_BRIGHTNESS_DEFAULT);
                    } else {
                        lights_led_set_brightness(&head->leds[i],LIGHTS_LED_BRIGHTNESS_OFF);
                    }
                }

                head->strobe_step++;
                head->fsm.last_transition_ms = curr_time_ms;  // Account for multiple cycles

                if (head->strobe_step >= STROBE_PATTERN_TIMING_LEN) {
                    head->strobe_step = 0;
                }
            }
            break;
        }

        case LIGHTS_STATE_OFF: {
            for (uint8_t i = 0; i < NUM_HEAD_LEDS; i++) {
                lights_led_set_brightness(&head->leds[i], LIGHTS_LED_BRIGHTNESS_OFF);
            }
            break;
        }
    }

    for (uint8_t i = 0; i < NUM_HEAD_LEDS; i++) {
        lights_led_update(&head->leds[i]);
    }

    lights_button_update(&head->button, utils_uptime_ms());

    if (lights_button_is_pressed(&head->button)) {
        lights_fsm_update(&head->fsm, LIGHTS_EVENT_BUTTON_PRESS);
    }
}