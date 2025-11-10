#pragma once

#include "config.h"
#include "fsm.h"
#include "button.h"
#include "led.h"

#define NUM_HEAD_LEDS 4

typedef struct {
    LightsFsm fsm;
    LightsButton button;
    LightsLed leds[NUM_HEAD_LEDS];
} HeadLight;

void head_light_init(HeadLight *head);
void head_light_update(HeadLight *head);