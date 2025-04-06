#ifndef SOIL_SENSOR_H
#define SOIL_SENSOR_H
#include <cstdint>

/**
 * Initialize the sensor
 */
void init_soil_sensor();

/**
 * Measures the soil moisture and returns it
 * as a uint8 in a range of 0..100
 */
uint8_t get_moisture_percentage();

#endif // SOIL_SENSOR_H