#ifndef WIFI_FUNCTIONS_H
#define WIFI_FUNCTIONS_H
#include <cstdint>
#include <Arduino.h>
#include <WiFi.h>

const char* encryptionTypeToString(wifi_auth_mode_t encryptionType);

/**
 * @brief Scans nearby networks and prints them to the logs
 *
 * @return uint32_t Number of networks found
 */
uint32_t wifiScan();

/**
 * @brief Scans for wifi networks and tries to connect to whatever network
 * is configured in the settings 
 */
void wifiSetup();

#endif // WIFI_FUNCTIONS_H