#include <Arduino.h>
#include "TelemetryCore.h"

void setup() {
    TelemetryCore::init();
}

void loop() {
    vTaskDelay(pdMS_TO_TICKS(1000));
}
