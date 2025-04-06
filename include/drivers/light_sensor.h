#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H
#include <cstdint>
/**
 * Initialize the sensor
 */
void init_light_sensor();

/**
 * Measures the ambient light level and returns it as an integer value in lux
 * @return The light level in lux
 * @return -1 If the return value is invalid
 * @return -2 If the sensor has not been configured
 */
int32_t get_lux_reading();

#endif // LIGHT_SENSOR_H