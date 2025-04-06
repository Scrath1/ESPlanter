#ifndef PUMP_H
#define PUMP_H

#include <stdint.h>

/**
 * Initialize pin for triggering the pump and software timer
 * for triggering the callback function to stop the pump
 * after the given run duration
 */
void init_pump();

/**
 * Run the pump for a set duration (milliseconds).
 * If given a duration of 0, the pump will be stopped in case
 * it is already running
 */
void run_pump(uint32_t duration_ms);

#endif // PUMP_H