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
    LightsState current_state;
    uint32_t last_transition_ms;
} LightsFsm;

typedef enum {
    LIGHTS_EVENT_NONE,
    LIGHTS_EVENT_BUTTON_PRESS,
    LIGHTS_EVENT_TIMER_ELAPSED
} LightsEvent;

void lights_fsm_init(LightsFsm *fsm);
void lights_fsm_update(LightsFsm *fsm, LightsEvent event);
LightsState lights_fsm_get_state(LightsFsm *fsm);