#include "drivers/soil_sensor.h"

#include <Arduino.h>

#include "global.h"
#include "static_config.h"

void soil_sensor_init() { pinMode(PIN_SOIL_SENSOR, INPUT); }

uint8_t soil_sensor_get_moisture_percentage() {
    uint16_t val = analogRead(PIN_SOIL_SENSOR);
    int16_t mappedVal = map(val, config.moisture_sensor_lower_limit, config.moisture_sensor_upper_limit, 100, 0);
    if(mappedVal > 100)
        mappedVal = 100;
    else if(mappedVal < 0)
        mappedVal = 0;
    return mappedVal;
}

uint16_t soil_sensor_get_moisture_raw() { return analogRead(PIN_SOIL_SENSOR); }