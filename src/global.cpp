#include "global.h"
#include "static_config.h"

#define _STRINGIFY(s) #s
#define STRINGIFY(s) _STRINGIFY(s)

ConfigStruct_t config;

ConfigEntry_t config_entries[] = {
    {STRINGIFY(config.wifi.ssid), CONFIG_STRING, &config.wifi.ssid, sizeof(config.wifi.ssid), CFG_PERM_RW},
    {STRINGIFY(config.wifi.password), CONFIG_STRING, &config.wifi.password, sizeof(config.wifi.password), CFG_PERM_SECRET_RW},
    {STRINGIFY(config.wifi.hostname), CONFIG_STRING, &config.wifi.hostname, sizeof(config.wifi.hostname), CFG_PERM_RW},
    {STRINGIFY(config.mqtt.broker_address), CONFIG_STRING, &config.mqtt.broker_address, sizeof(config.mqtt.broker_address), CFG_PERM_RW},
    {STRINGIFY(config.mqtt.broker_port), CONFIG_UINT32, &config.mqtt.broker_port, sizeof(config.mqtt.broker_port), CFG_PERM_RW},
    {STRINGIFY(config.mqtt.client_id), CONFIG_STRING, &config.mqtt.client_id, sizeof(config.mqtt.client_id), CFG_PERM_RW},
    {STRINGIFY(config.mqtt.username), CONFIG_STRING, &config.mqtt.username, sizeof(config.mqtt.username), CFG_PERM_RW},
    {STRINGIFY(config.mqtt.password), CONFIG_STRING, &config.mqtt.password, sizeof(config.mqtt.password), CFG_PERM_SECRET_RW},
    {STRINGIFY(config.mqtt.device_topic), CONFIG_STRING, &config.mqtt.device_topic, sizeof(config.mqtt.device_topic), CFG_PERM_RW},
    {STRINGIFY(config.mqtt.discovery_prefix), CONFIG_STRING, &config.mqtt.discovery_prefix, sizeof(config.mqtt.discovery_prefix), CFG_PERM_RW},
    {STRINGIFY(config.pump_duration_ms), CONFIG_STRING, &config.pump_duration_ms, sizeof(config.pump_duration_ms), CFG_PERM_RW}
};

ConfigTable_t config_table{
    .entries = config_entries,
    .count = sizeof(config_entries) / sizeof(config_entries[0])
};