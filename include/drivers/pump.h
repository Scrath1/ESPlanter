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
 * Run the pump for a set duration (milliseconds)
 */
void run_pump(uint32_t duration_ms);

#endif // PUMP_H