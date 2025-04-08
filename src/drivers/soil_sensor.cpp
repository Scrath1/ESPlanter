#include "drivers/soil_sensor.h"

#include <Arduino.h>

#include "static_config.h"

void init_soil_sensor() { pinMode(PIN_SOIL_SENSOR, INPUT); }

uint8_t get_moisture_percentage() {
    uint16_t val = analogRead(PIN_SOIL_SENSOR);
    // ToDo: Make the limits configurable at runtime
    int16_t mappedVal = map(val, MOISTURE_LOWER_LIMIT, MOISTURE_UPPER_LIMIT, 100, 0);
    if(mappedVal > 100)
        mappedVal = 100;
    else if(mappedVal < 0)
        mappedVal = 0;
    return mappedVal;
}