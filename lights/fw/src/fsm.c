#include "fsm.h"

void lights_fsm_init(LightsFsm *fsm, const LightsState *state_sequence, uint8_t num_states) {
    fsm->state_sequence = state_sequence;
    fsm->num_states = num_states;
    fsm->index = 0;
    fsm->last_transition_ms = 0;  // TODO: Define using a millis() implementation
}

void lights_fsm_update(LightsFsm *fsm, LightsEvent event) {
    if (event == LIGHTS_EVENT_BUTTON_PRESS) {
        fsm->index++;

        if (fsm->index >= fsm->num_states) {
            fsm->index = 0;  // Wrap around to first state
        }

        fsm->last_transition_ms = 0;  // TODO: Define using a millis() implementation
    }

    // NOTE: OLD IMPLEMENTATION -> Use something of the sort a layer higher in head/tail.c 
    // switch(fsm->current_state) {
    //     case LIGHTS_STATE_STARTUP:
    //         if (event == LIGHTS_EVENT_TIMER_ELAPSED) {
    //             lights_fsm_set_state(fsm, LIGHTS_STATE_SOLID_BRIGHT);
    //         }
    //         break;

    //     case LIGHTS_STATE_SOLID_BRIGHT:
    //         if (event = LIGHTS_EVENT_BUTTON_PRESS) {
    //             lights_fsm_set_state(fsm, LIGHTS_STATE_SOLID_DIM);
    //         }
    //         break;

    //     case LIGHTS_STATE_SOLID_DIM:
    //         if (event = LIGHTS_EVENT_BUTTON_PRESS) {
    //             lights_fsm_set_state(fsm, LIGHTS_STATE_FLASH);
    //         }
    //         break;

    //     case LIGHTS_STATE_FLASH:
    //         if (event = LIGHTS_EVENT_BUTTON_PRESS) {
    //             lights_fsm_set_state(fsm, LIGHTS_STATE_STROBE);
    //         }
    //         break;

    //     case LIGHTS_STATE_STROBE:
    //         if (event = LIGHTS_EVENT_BUTTON_PRESS) {
    //             lights_fsm_set_state(fsm, LIGHTS_STATE_OFF);
    //         }
    //         break;

    //     case LIGHTS_STATE_OFF:
    //         if (event = LIGHTS_EVENT_BUTTON_PRESS) {
    //             lights_fsm_set_state(fsm, LIGHTS_STATE_STARTUP);
    //         }
    //         break;

    //     default:
    //         lights_fsm_set_state(fsm, LIGHTS_STATE_STARTUP);
    //         break;
    // }
}

LightsState lights_fsm_get_state(LightsFsm *fsm) {
    return fsm->state_sequence[fsm->index];
}