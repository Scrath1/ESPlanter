#include "drivers/pump.h"

#include <Arduino.h>

#include "static_config.h"

#define PUMP_TIMER_NAME "Pump Cb Timer"
#define PUMP_ACTIVE_LEVEL 1
#define PUMP_INACTIVE_LEVEL 0

TimerHandle_t pump_callback_timer;
bool pump_running = false;

void pump_timer_callback_func(TimerHandle_t xtimer) {
    digitalWrite(PIN_PUMP, PUMP_INACTIVE_LEVEL);
    pump_running = false;
}

void pump_init() {
    pinMode(PIN_PUMP, OUTPUT);
    digitalWrite(PIN_PUMP, PUMP_INACTIVE_LEVEL);
    pump_running = false;
    pump_callback_timer = xTimerCreate(PUMP_TIMER_NAME, 1000, pdFALSE, 0, pump_timer_callback_func);
}

void pump_run(uint32_t duration_ms) {
    // Stop timer in case it was running and set new time until callback
    if(duration_ms == 0) {
        digitalWrite(PIN_PUMP, PUMP_INACTIVE_LEVEL);
        xTimerStop(pump_callback_timer, portMAX_DELAY);
        pump_running = false;
    } else {
        xTimerStop(pump_callback_timer, portMAX_DELAY);
        xTimerChangePeriod(pump_callback_timer, pdMS_TO_TICKS(duration_ms), portMAX_DELAY);
        xTimerStart(pump_callback_timer, portMAX_DELAY);

        digitalWrite(PIN_PUMP, PUMP_ACTIVE_LEVEL);
        pump_running = true;
    }
}

bool pump_is_running() { return pump_running; }