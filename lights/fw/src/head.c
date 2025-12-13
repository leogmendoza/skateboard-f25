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

void head_light_init(HeadLight *head) {
    lights_fsm_init(&head->fsm, head_state_sequence, NUM_STATES);
    lights_button_init(&head->button, HEAD_BUTTON_PIN, HEAD_BUTTON_DDR, HEAD_BUTTON_PORT, HEAD_BUTTON_BIT);

    for (uint8_t i = 0; i < NUM_HEAD_LEDS; i++) {
        lights_led_init(&head->leds[i], HEAD_LEDS_DDR, HEAD_LEDS_PORT, head_bit_sequence[i], NO_PWM_CH);
    }
}

void head_light_update(HeadLight *head) {
    switch (lights_fsm_get_state(&head->fsm)) {
        case LIGHTS_STATE_STARTUP:
            uint32_t curr_time_ms = utils_uptime_ms();
            uint32_t delta_time_ms = curr_time_ms - head->fsm.last_transition_ms;

            for (uint8_t i = 0; i < NUM_HEAD_LEDS; i++) {
                if (delta_time_ms >= (i * STARTUP_SEQUENCE_INTERVAL_MS)) {
                    lights_led_set_brightness(&head->leds[i], LIGHTS_LED_BRIGHTNESS_DEFAULT);
                } else {
                    lights_led_set_brightness(&head->leds[i], LIGHTS_LED_BRIGHTNESS_OFF);
                }
            }

            // Automatically transition to next state after repeating the sequence
            if ( delta_time_ms > (STARTUP_SEQUENCE_ITERATIONS * NUM_HEAD_LEDS * STARTUP_SEQUENCE_INTERVAL_MS) ) {
                lights_fsm_update(&head->fsm, LIGHTS_EVENT_STARTUP_COMPLETE);
            }

            break;

        case LIGHTS_STATE_SOLID:
            for (uint8_t i = 0; i < NUM_HEAD_LEDS; i++) {
                lights_led_set_brightness(&head->leds[i], LIGHTS_LED_BRIGHTNESS_DEFAULT);
            }
            break;

        case LIGHTS_STATE_STROBE:
            for (uint8_t i = 0; i < NUM_HEAD_LEDS; i++) {
                lights_led_set_brightness( &head->leds[i], !(head->leds[i].brightness) );
            }

            // TODO: Figure out how to strobe
            break;

        case LIGHTS_STATE_OFF:
            for (uint8_t i = 0; i < NUM_HEAD_LEDS; i++) {
                lights_led_set_brightness(&head->leds[i], LIGHTS_LED_BRIGHTNESS_OFF);
            }
            break;
    }

    for (uint8_t i = 0; i < NUM_HEAD_LEDS; i++) {
        lights_led_update(&head->leds[i]);
    }

    lights_button_update(&head->button, utils_uptime_ms());

    if (lights_button_is_pressed(&head->button)) {
        lights_fsm_update(&head->fsm, LIGHTS_EVENT_BUTTON_PRESS);
    }
}