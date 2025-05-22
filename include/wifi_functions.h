#ifndef WIFI_FUNCTIONS_H
#define WIFI_FUNCTIONS_H
#include <Arduino.h>
#include <WiFi.h>

#include <cstdint>

const char* encryption_type_to_string(wifi_auth_mode_t encryptionType);

/**
 * @brief Scans nearby networks and prints them to the logs
 *
 * @return uint32_t Number of networks found
 */
uint32_t wifi_scan();

/**
 * @brief Scans for wifi networks and tries to connect to whatever network
 * is configured in the settings
 * @return true on successfully establishing a wifi connection
 * @return false on no connection
 */
bool wifi_setup();

#endif  // WIFI_FUNCTIONS_H