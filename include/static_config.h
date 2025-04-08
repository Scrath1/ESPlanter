#ifndef STATIC_CONFIG_H
#define STATIC_CONFIG_H

/**
 * The purpose of this file is to declare macros for compile-time configuration
 * which can not be changed at runtime, e.g. pinouts
 */

 // Pump pin
#define PIN_PUMP (D10)
#define DEFAULT_PUMP_DURATION_MS (3000)
// Soil sensor pin
#define PIN_SOIL_SENSOR (A0)
#define MOISTURE_LOWER_LIMIT (1196)
#define MOISTURE_UPPER_LIMIT (3367)
// Absolute maximum continuous runtime of the pump in milliseconds
#define MAX_PUMP_RUNTIME_MS (60000)
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

#define SERIAL_CMD_INPUT_BUFFER_SIZE (256)
#define MAX_TOPIC_STRING_SIZE (256)

// Name of the general configuration file. Keep in mind that
// LittleFS requires absolute file paths.
#define CONFIG_FILE_NAME "/config.txt"

// Time in ms after which the ESP will give up trying to connect to an WiFi station
#define WIFI_CONNECTION_TIMEOUT_MS 5000

// MQTT configuration
// ============================================

// Defines in seconds how long to keep the mqtt connection open when
// no packages are being sent or received
#define MQTT_CONNECTION_KEEPALIVE_S 15
#define MQTT_BASE_TOPIC "ESPlanter"
// Default interval for how often updated sensor readings are sent to the server
#define MQTT_DEFAULT_UPDATE_INTERVAL_MS (15000)
#define MQTT_CONNECTION_RETRY_INTERVAL_S (15)

#endif // STATIC_CONFIG_H