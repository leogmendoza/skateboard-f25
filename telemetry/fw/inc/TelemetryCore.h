#pragma once

#include <Arduino.h>

#define ONBOARD_LED_1 18
#define ONBOARD_LED_2 19

// Core telemetry orchestration entrypoint
namespace TelemetryCore {
    void init();
}
