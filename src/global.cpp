#include "global.h"
#include "static_config.h"

#define _STRINGIFY(s) #s
#define STRINGIFY(s) _STRINGIFY(s)

char wifi_ssid[MAX_WIFI_SSID_LEN] = "";
char wifi_password[MAX_WIFI_PASSWORD_LEN] = "";
char wifi_hostname[MAX_WIFI_HOSTNAME_LEN] = "";
char mqtt_broker_address[MAX_MQTT_BROKER_ADDRESS_LEN] = "";
uint32_t mqtt_broker_port = 1883;
char mqtt_client_id[MAX_MQTT_CLIENT_ID_LEN] = "";
char mqtt_username[MAX_MQTT_USERNAME_LEN] = "";
char mqtt_password[MAX_MQTT_PASSWORD_LEN] = "";
char mqtt_device_topic[MAX_MQTT_DEVICE_TOPIC_LEN] = "";

ConfigEntry_t config_entries[] = {
    {STRINGIFY(wifi_ssid), CONFIG_STRING, &wifi_ssid, sizeof(wifi_ssid), CFG_PERM_RW},
    {STRINGIFY(wifi_password), CONFIG_STRING, &wifi_password, sizeof(wifi_password), CFG_PERM_SECRET_RW},
    {STRINGIFY(wifi_hostname), CONFIG_STRING, &wifi_hostname, sizeof(wifi_hostname), CFG_PERM_RW},
    {STRINGIFY(mqtt_broker_address), CONFIG_STRING, &mqtt_broker_address, sizeof(mqtt_broker_address), CFG_PERM_RW},
    {STRINGIFY(mqtt_broker_port), CONFIG_UINT32, &mqtt_broker_port, sizeof(mqtt_broker_port), CFG_PERM_RW},
    {STRINGIFY(mqtt_client_id), CONFIG_STRING, &mqtt_client_id, sizeof(mqtt_client_id), CFG_PERM_RW},
    {STRINGIFY(mqtt_username), CONFIG_STRING, &mqtt_username, sizeof(mqtt_username), CFG_PERM_RW},
    {STRINGIFY(mqtt_password), CONFIG_STRING, &mqtt_password, sizeof(mqtt_password), CFG_PERM_SECRET_RW},
    {STRINGIFY(mqtt_device_topic), CONFIG_STRING, &mqtt_device_topic, sizeof(mqtt_device_topic), CFG_PERM_RW}
};

ConfigTable_t config{
    .entries = config_entries,
    .count = sizeof(config_entries) / sizeof(config_entries[0])
};