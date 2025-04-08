#ifndef PUMP_H
#define PUMP_H

#include <stdint.h>

/**
 * Initialize pin for triggering the pump and software timer
 * for triggering the callback function to stop the pump
 * after the given run duration
 */
void pump_init();

/**
 * Run the pump for a set duration (milliseconds).
 * If given a duration of 0, the pump will be stopped in case
 * it is already running
 */
void pump_run(uint32_t duration_ms);

bool pump_is_running();

#endif  // PUMP_H