#ifndef STATIC_CONFIG_H
#define STATIC_CONFIG_H

/**
 * The purpose of this file is to declare macros for compile-time configuration
 * which can not be changed at runtime, e.g. pinouts
 */

 // Pump pin
#define PIN_PUMP (D10)
// Maximum length of the SSID string including null-terminator
#define MAX_WIFI_SSID_LEN (64)
// Maximum length of the password string including null-terminator
#define MAX_WIFI_PASSWORD_LEN (64)
// Maximum length of the wifi hostname string including null-terminator
#define MAX_WIFI_HOSTNAME_LEN (64)
// Maximum length of the mqtt broker address string including null-terminator
#define MAX_MQTT_BROKER_ADDRESS_LEN (64)
#define MAX_MQTT_CLIENT_ID_LEN (64)
#define MAX_MQTT_USERNAME_LEN (64)
#define MAX_MQTT_PASSWORD_LEN (64)
#define MAX_MQTT_DEVICE_TOPIC_LEN (64)

#define CONFIG_FILE_NAME "/config.txt"

#endif // STATIC_CONFIG_H