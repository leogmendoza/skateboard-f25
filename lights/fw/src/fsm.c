#include "fsm.h"

static void lights_fsm_set_state(LightsFsm *lights_fsm, LightsState new_state) {
    lights_fsm->current_state = new_state;
    lights_fsm->last_transition_ms = 0;  // TODO: Define using a millis() implementation
}

void lights_fsm_init(LightsFsm *lights_fsm) {
    lights_fsm->current_state = LIGHTS_STATE_STARTUP;
    lights_fsm->last_transition_ms = 0;  // TODO: Define using a millis() implementation
}

void lights_fsm_update(LightsFsm *lights_fsm, LightsEvent lights_event) {
    switch(lights_fsm->current_state) {
        case LIGHTS_STATE_STARTUP:
            if (lights_event == LIGHTS_EVENT_TIMER_ELAPSED) {
                lights_fsm_set_state(lights_fsm, LIGHTS_STATE_SOLID_BRIGHT);
            }
            break;

        case LIGHTS_STATE_SOLID_BRIGHT:
            if (lights_event = LIGHTS_EVENT_BUTTON_PRESS) {
                lights_fsm_set_state(lights_fsm, LIGHTS_STATE_SOLID_DIM);
            }
            break;

        case LIGHTS_STATE_SOLID_DIM:
            if (lights_event = LIGHTS_EVENT_BUTTON_PRESS) {
                lights_fsm_set_state(lights_fsm, LIGHTS_STATE_FLASH);
            }
            break;

        case LIGHTS_STATE_FLASH:
            if (lights_event = LIGHTS_EVENT_BUTTON_PRESS) {
                lights_fsm_set_state(lights_fsm, LIGHTS_STATE_STROBE);
            }
            break;

        case LIGHTS_STATE_STROBE:
            if (lights_event = LIGHTS_EVENT_BUTTON_PRESS) {
                lights_fsm_set_state(lights_fsm, LIGHTS_STATE_OFF);
            }
            break;

        case LIGHTS_STATE_OFF:
            if (lights_event = LIGHTS_EVENT_BUTTON_PRESS) {
                lights_fsm_set_state(lights_fsm, LIGHTS_STATE_STARTUP);
            }
            break;

        default:
            lights_fsm_set_state(lights_fsm, LIGHTS_STATE_STARTUP);
            break;
    }
}

LightsState lights_fsm_get_state(LightsFsm *lights_fsm) {
    return lights_fsm->current_state;
}