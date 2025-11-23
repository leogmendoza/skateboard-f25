#include "tail.h"
#include "fsm.h"

static const LightsState TAIL_SEQUENCE[] = {
    LIGHTS_STATE_STARTUP,
    LIGHTS_STATE_SOLID,
    LIGHTS_STATE_FLASH,
    LIGHTS_STATE_OFF,
};