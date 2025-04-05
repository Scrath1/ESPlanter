#ifndef GLOBAL_H
#define GLOBAL_H
#include <config_table.h>
#include "static_config.h"

typedef struct{
    struct {
        char ssid[MAX_WIFI_SSID_LEN] = "";
        char password[MAX_WIFI_PASSWORD_LEN] = "";
        char hostname[MAX_WIFI_HOSTNAME_LEN] = "";
    } wifi;
    struct {
        char broker_address[MAX_MQTT_BROKER_ADDRESS_LEN] = "";
        uint32_t broker_port = 1883;
        char client_id[MAX_MQTT_CLIENT_ID_LEN] = "";
        char username[MAX_MQTT_USERNAME_LEN] = "";
        char password[MAX_MQTT_PASSWORD_LEN] = "";
        char device_topic[MAX_MQTT_DEVICE_TOPIC_LEN] = "";
    } mqtt;
} ConfigStruct_t;

extern ConfigTable_t config_table;
extern ConfigStruct_t config;

#endif // GLOBAL_H