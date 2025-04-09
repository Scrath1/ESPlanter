#ifndef SOIL_SENSOR_H
#define SOIL_SENSOR_H
#include <cstdint>

/**
 * Initialize the sensor
 */
void soil_sensor_init();

/**
 * Measures the soil moisture and returns it
 * as a uint8 in a range of 0..100
 */
uint8_t soil_sensor_get_moisture_percentage();

/**
 * Returns the raw ADC reading of the moisture sensor
 */
uint16_t soil_sensor_get_moisture_raw();

#endif  // SOIL_SENSOR_H