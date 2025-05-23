#ifndef GLOBAL_H
#define GLOBAL_H
#include <config_table.h>

#include "static_config.h"

typedef struct {
    struct {
        char ssid[MAX_WIFI_SSID_LEN] = "";
        char password[MAX_WIFI_PASSWORD_LEN] = "";
        char hostname[MAX_WIFI_HOSTNAME_LEN] = "";
    } wifi;
    struct {
        char broker_address[MAX_MQTT_BROKER_ADDRESS_LEN] = "0.0.0.0";
        uint32_t broker_port = 1883;
        char client_id[MAX_MQTT_CLIENT_ID_LEN] = "";
        char username[MAX_MQTT_USERNAME_LEN] = "";
        char password[MAX_MQTT_PASSWORD_LEN] = "";
        char device_topic[MAX_MQTT_DEVICE_TOPIC_LEN] = "";  // leave empty, this will be filled automatically
        char discovery_prefix[MAX_MQTT_DISCOVERY_TOPIC_LEN] = "homeassistant";
    } mqtt;
    uint32_t pump_duration_ms = DEFAULT_PUMP_DURATION_MS;
    uint32_t moisture_sensor_lower_limit = MOISTURE_RAW_LOWER_LIMIT;
    uint32_t moisture_sensor_upper_limit = MOISTURE_RAW_UPPER_LIMIT;
} ConfigStruct_t;

extern ConfigTable_t config_table;
extern ConfigStruct_t config;

#endif  // GLOBAL_H