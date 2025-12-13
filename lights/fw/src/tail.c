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
}

void tail_light_update(TailLight *tail) {
    // TODO: Implement
}