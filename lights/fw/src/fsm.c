#include "fsm.h"

static void lights_fsm_set_state(LightsFsm *fsm, LightsState new_state) {
    fsm->current_state = new_state;
    fsm->last_transition_ms = 0;  // TODO: Define using a millis() implementation
}

void lights_fsm_init(LightsFsm *fsm) {
    fsm->current_state = LIGHTS_STATE_STARTUP;
    fsm->last_transition_ms = 0;  // TODO: Define using a millis() implementation
}

void lights_fsm_update(LightsFsm *fsm, LightsEvent event) {
    switch(fsm->current_state) {
        case LIGHTS_STATE_STARTUP:
            if (event == LIGHTS_EVENT_TIMER_ELAPSED) {
                lights_fsm_set_state(fsm, LIGHTS_STATE_SOLID_BRIGHT);
            }
            break;

        case LIGHTS_STATE_SOLID_BRIGHT:
            if (event = LIGHTS_EVENT_BUTTON_PRESS) {
                lights_fsm_set_state(fsm, LIGHTS_STATE_SOLID_DIM);
            }
            break;

        case LIGHTS_STATE_SOLID_DIM:
            if (event = LIGHTS_EVENT_BUTTON_PRESS) {
                lights_fsm_set_state(fsm, LIGHTS_STATE_FLASH);
            }
            break;

        case LIGHTS_STATE_FLASH:
            if (event = LIGHTS_EVENT_BUTTON_PRESS) {
                lights_fsm_set_state(fsm, LIGHTS_STATE_STROBE);
            }
            break;

        case LIGHTS_STATE_STROBE:
            if (event = LIGHTS_EVENT_BUTTON_PRESS) {
                lights_fsm_set_state(fsm, LIGHTS_STATE_OFF);
            }
            break;

        case LIGHTS_STATE_OFF:
            if (event = LIGHTS_EVENT_BUTTON_PRESS) {
                lights_fsm_set_state(fsm, LIGHTS_STATE_STARTUP);
            }
            break;

        default:
            lights_fsm_set_state(fsm, LIGHTS_STATE_STARTUP);
            break;
    }
}

LightsState lights_fsm_get_state(LightsFsm *fsm) {
    return fsm->current_state;
}