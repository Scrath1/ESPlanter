#include "drivers/soil_sensor.h"
#include <Arduino.h>
#include "static_config.h"

void init_soil_sensor(){
    pinMode(PIN_SOIL_SENSOR, INPUT);
}

uint8_t get_moisture_percentage(){
    uint16_t val = analogRead(PIN_SOIL_SENSOR);
    return map(val, MOISTURE_LOWER_LIMIT, MOISTURE_UPPER_LIMIT, 100, 0);
}