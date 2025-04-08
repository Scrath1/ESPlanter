#include "drivers/light_sensor.h"

#include <BH1750.h>
#include <Wire.h>

BH1750 light_sensor;

void init_light_sensor() {
    if(!Wire.begin()) {
        Serial.println("Failed to initialize I2C bus");
    }
    if(!light_sensor.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
        Serial.println("Failed to initialize light sensor");
    }
}

int32_t get_lux_reading() { return static_cast<int32_t>(light_sensor.readLightLevel()); }