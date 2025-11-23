#include "head.h"
#include "fsm.h"

static const LightsState HEAD_SEQUENCE[] = {
    LIGHTS_STATE_STARTUP,
    LIGHTS_STATE_SOLID,
    LIGHTS_STATE_STROBE,
    LIGHTS_STATE_OFF,
};