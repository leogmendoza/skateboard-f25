#pragma once

#include "config.h"
#include "fsm.h"
#include "button.h"
#include "led.h"

typedef struct {
    LightsFsm fsm;
    LightsButton button;
    LightsLed leds[NUM_TAIL_LEDS];
} TailLight;

void tail_light_init(TailLight *tail);
void tail_light_update(TailLight *tail);