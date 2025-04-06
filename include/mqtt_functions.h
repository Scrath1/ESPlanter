#ifndef MQTT_FUNCTIONS_H
#define MQTT_FUNCTIONS_H
#include <WiFi.h>
#include <PubSubClient.h>
#include "global.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

extern PubSubClient mqttClient;

/**
 * Runs the MQTT setup process, thereby checking the validity of
 * the configured broker address to determine whether an MQTT connection
 * is desired. If it is, a task for managing the connection is established.
 * 
 * This function can be re-run multiple times without issues
 */
void mqttSetup();

/**
 * Checks whether the MQTT client is still connected.
 * If not, an attempt to reconnect is made.
 * On successful reconnect, topic subscriptions are renewed
 */
void mqttMaintainConnection();

#endif // MQTT_FUNCTIONS_H