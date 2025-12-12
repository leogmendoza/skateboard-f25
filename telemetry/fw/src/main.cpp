#include <Arduino.h>
#include "TelemetryCore.h"

void setup() {
    delay(1000);
    TelemetryCore::init();
}

void loop() {
    vTaskDelay(pdMS_TO_TICKS(1000));
}
