#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    LIGHTS_STATE_STARTUP,
    LIGHTS_STATE_SOLID_BRIGHT,
    LIGHTS_STATE_SOLID_DIM,
    LIGHTS_STATE_FLASH,
    LIGHTS_STATE_STROBE,
    LIGHTS_STATE_OFF,
    NUM_STATES
} LightsState;

typedef struct {
    const LightsState *state_sequence;
    uint8_t num_states;
    uint8_t index;
    uint32_t last_transition_ms;
} LightsFsm;

typedef enum {
    LIGHTS_EVENT_NONE,
    LIGHTS_EVENT_BUTTON_PRESS,
    LIGHTS_EVENT_TIMER_ELAPSED
} LightsEvent;

/**
 * @brief Initialize FSM with its state sequence
 */
void lights_fsm_init(LightsFsm *fsm, const LightsState *state_sequence, uint8_t num_states);

/**
 * @brief Advance FSM to the next state on an event
 */
void lights_fsm_update(LightsFsm *fsm, LightsEvent event);

/**
 * @brief Return the FSM’s current state
 */
LightsState lights_fsm_get_state(LightsFsm *fsm);