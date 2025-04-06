#ifndef MQTT_TASK_H
#define MQTT_TASK_H
#include <WiFi.h>
#include <PubSubClient.h>
#include "global.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define MQTT_TASK_NAME ("MQTT_Task")
#define MQTT_TASK_STACK_SIZE (2048)
#define MQTT_TASK_PRIORITY (configMAX_PRIORITIES / 2 + 1)

extern PubSubClient mqttClient;

/**
 * Runs the MQTT setup process, thereby checking the validity of
 * the configured broker address to determine whether an MQTT connection
 * is desired. If it is, a task for managing the connection is established.
 * 
 * This function can be re-run multiple times without issues
 */
void mqttSetup();

#endif // MQTT_TASK_H