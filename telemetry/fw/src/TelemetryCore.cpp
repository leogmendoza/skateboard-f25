#include "TelemetryCore.h"

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "BmsAPI.h"             // BMS wrapper API
#include "vescParser.hpp"       // VESC data
#include "oled_display_API.h"   // OLED display
#include "ble_handler.h"        // BLE server & notifications

// ========= Configuration =========
#define RS485_DIR_PIN              22
#define BMS_UART_BAUD              9600

#define BMS_TASK_PERIOD_MS         100     // how often to poll BMS
#define VESC_TASK_PERIOD_MS        100     // how often to poll VESC
#define OLED_TASK_PERIOD_MS        200     // display refresh
#define BLE_TASK_PERIOD_MS         250     // BLE notification rate

// current wheel dimensions set 90mm
static constexpr float WHEEL_DIAMETER_M        = 0.105f;
static constexpr float WHEEL_CIRCUMFERENCE_M   = WHEEL_DIAMETER_M * PI;          
static constexpr float KMH_PER_RPM             = (WHEEL_CIRCUMFERENCE_M / 1000.0f) * 60.0f;
static constexpr float KM_PER_TACH_COUNT       = WHEEL_CIRCUMFERENCE_M / 1000.0f;

// ========= Global API instances =========
static BmsAPI     gBms(Serial2);  
static VescParser gVesc;

// ========= Shared Telemetry =========
struct TelemetryData {
    float speedKmh      = 0.0f;   // from VESC RPM
    float distanceKm    = 0.0f;   // from VESC tachometerAbs

    float socPercent    = 0.0f;   // from BMS
    float packVoltage   = 0.0f;   // from BMS
    float packCurrent   = 0.0f;   // from BMS
    float tempC         = 0.0f;   // from BMS
};

static TelemetryData     gTelemetry;
static SemaphoreHandle_t gTelemetryMutex = nullptr;


static TelemetryData getTelemetrySnapshot() {
    TelemetryData snap;
    if (xSemaphoreTake(gTelemetryMutex, portMAX_DELAY) == pdTRUE) {
        snap = gTelemetry;
        xSemaphoreGive(gTelemetryMutex);
    }
    return snap;
}

// ========= BMS Task: updates BMS metrics =========
static void bmsTask(void *pvParameters) {
    (void) pvParameters;

    for (;;) {
        gBms.update();   // internal JbdBms call

        float voltage = gBms.getVoltage();               // V
        float current = gBms.getCurrent();               // A
        float soc     = gBms.getChargePercentage();      // %
        float temp    = gBms.getAverageTemperature();    // °C (avg of NTCs):contentReference[oaicite:0]{index=0}

        if (xSemaphoreTake(gTelemetryMutex, portMAX_DELAY) == pdTRUE) {
            gTelemetry.packVoltage = voltage;
            gTelemetry.packCurrent = current;
            gTelemetry.socPercent  = soc;
            gTelemetry.tempC       = temp;
            xSemaphoreGive(gTelemetryMutex);
        }

        vTaskDelay(pdMS_TO_TICKS(BMS_TASK_PERIOD_MS));
    }
}

// ========= VESC Task: updates speed & distance =========
static void vescTask(void *pvParameters) {
    (void) pvParameters;

    for (;;) {
        gVesc.getData();  // updates internal data.rpm, data.tachometerAbs, etc.:contentReference[oaicite:1]{index=1}

        float rpm   = gVesc.getRPM();
        int32_t tach = gVesc.getTachometerAbs();

        // Convert to speed (km/h) and distance (km)
        float speedKmh   = rpm * KMH_PER_RPM;
        float distanceKm = static_cast<float>(tach) * KM_PER_TACH_COUNT;

        if (xSemaphoreTake(gTelemetryMutex, portMAX_DELAY) == pdTRUE) {
            gTelemetry.speedKmh   = speedKmh;
            gTelemetry.distanceKm = distanceKm;
            xSemaphoreGive(gTelemetryMutex);
        }

        vTaskDelay(pdMS_TO_TICKS(VESC_TASK_PERIOD_MS));
    }
}

// ========= OLED Task: show speed + distance only =========
static void oledTask(void *pvParameters) {
    (void) pvParameters;

    for (;;) {
        TelemetryData t = getTelemetrySnapshot();

        // Show speed, distance, and battery % on the OLED
        update_speed_distance(t.speedKmh, t.distanceKm, t.socPercent);

        vTaskDelay(pdMS_TO_TICKS(OLED_TASK_PERIOD_MS));
    }
}

// ========= BLE Task: send limited telemetry =========
// pCharacteristic1..4 & notifyBLEData are defined in ble_handler.cpp:contentReference[oaicite:2]{index=2}
static void bleTask(void *pvParameters) {
    (void) pvParameters;

    for (;;) {
        if (isClientConnected()) {
            TelemetryData t = getTelemetrySnapshot();

            // Char 1: Speed (km/h)
            notifyBLEData(pCharacteristic1, String(t.speedKmh, 1));

            // Char 2: Distance (km)
            notifyBLEData(pCharacteristic2, String(t.distanceKm, 2));

            // Char 3: Battery % (SoC)
            notifyBLEData(pCharacteristic3, String(t.socPercent, 1));

            // Char 4: Pack Voltage (V)
            notifyBLEData(pCharacteristic4, String(t.packVoltage, 2));
        }

        vTaskDelay(pdMS_TO_TICKS(BLE_TASK_PERIOD_MS));
    }
}

// ========= Public init() entrypoint =========
namespace TelemetryCore {

void init() {
    // Serial mostly for debugging; BLE also does Serial.begin internally
    Serial.begin(115200);
    delay(300);

    Serial.println();
    Serial.println("========== TelemetryCore Init ==========");

    // Mutex for shared telemetry
    gTelemetryMutex = xSemaphoreCreateMutex();
    if (!gTelemetryMutex) {
        Serial.println("[FATAL] Failed to create telemetry mutex");
        while (true) { delay(1000); }
    }

    // OLED init
    Serial.println("[Core] Initializing OLED...");
    display_init();
    display_refresh(); 

    // BMS init
    Serial.println("[Core] Initializing BMS...");
    if (!gBms.begin(BMS_UART_BAUD)) {
        Serial.println("[WARN] BMS begin() failed");
        warning_updateOLED("BMS init failed");
    }

    // VESC init
    Serial.println("[Core] Initializing VESC parser...");
    gVesc.initialize();

    // BLE init
    Serial.println("[Core] Initializing BLE...");
    initBLE();

    // Create tasks
    Serial.println("[Core] Creating FreeRTOS tasks...");

    xTaskCreate(bmsTask,  "BMS Task",  4096, nullptr, 3, nullptr);
    xTaskCreate(vescTask, "VESC Task", 4096, nullptr, 3, nullptr);
    xTaskCreate(oledTask, "OLED Task", 4096, nullptr, 2, nullptr);
    xTaskCreate(bleTask,  "BLE Task",  4096, nullptr, 2, nullptr);

    Serial.println("[Core] TelemetryCore init complete.");
}

}
