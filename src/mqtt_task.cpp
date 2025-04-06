#include "mqtt_task.h"

TaskHandle_t mqttTaskHandle = nullptr;
WiFiClient espWiFiClient;
PubSubClient mqttClient(espWiFiClient);

void reconnect() {
    // Loop until we're reconnected
    while(!mqttClient.connected()) {
        Serial.printf("Attempting MQTT connection, ID: %s, User: %s\n", config.mqtt.client_id, config.mqtt.username);
        // Attempt to connect
        if(mqttClient.connect(config.mqtt.client_id, config.mqtt.username, config.mqtt.password)) {
            Serial.println("connected");
        } else {
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

/**
 * @brief MQTT Task function. Should never return.
 * The job of this function is to set up the mqtt client and periodically check
 * its connection to the server. If it got disconnected, reconnect.
 *
 * Cycle time: MQTT_TASK_CYCLE_TIME_MS
 *
 * @param pvParameters
 */
void mqttTask(void* pvParameters) {
    TickType_t lastWakeTime;

    // setup
    mqttClient.setKeepAlive(MQTT_CONNECTION_KEEPALIVE_S);
    IPAddress serverIP;
    serverIP.fromString(config.mqtt.broker_address);
    Serial.printf("MQTT broker address: %s\n", serverIP.toString().c_str());
    mqttClient.setServer(serverIP, config.mqtt.broker_port);

    while(1) {
        // Get current tick count for more precise cycle time calculation
        lastWakeTime = xTaskGetTickCount();

        if(WiFi.isConnected()) {
            // Process messages and maintain connection
            if(!mqttClient.connected()) {
                // lost connection. Try to reconnect...
                int8_t err = mqttClient.connect(config.mqtt.client_id, config.mqtt.username, config.mqtt.password);
                bool stopFlag = false;
                switch(err) {
                    default:
                        break;
                    case MQTT_CONNECTION_TIMEOUT:
                        Serial.println("MQTT connection timeout");
                        break;
                    case MQTT_CONNECTION_LOST:
                        Serial.println("MQTT connection lost");
                        break;
                    case MQTT_CONNECT_FAILED:
                        Serial.println("MQTT connection failed");
                        break;
                    case MQTT_CONNECT_BAD_PROTOCOL:
                        Serial.println("MQTT version not supported by server");
                        break;
                    case MQTT_CONNECT_BAD_CLIENT_ID:
                        Serial.println("MQTT client ID rejected by server");
                        break;
                    case MQTT_CONNECT_UNAVAILABLE:
                        Serial.println("MQTT server unavailable");
                        break;
                    case MQTT_CONNECT_BAD_CREDENTIALS:
                        Serial.println("MQTT credentials rejected");
                        stopFlag = true;
                        break;
                    case MQTT_CONNECT_UNAUTHORIZED:
                        Serial.println("MQTT client not authorized");
                        stopFlag = true;
                        break;
                }
                if(stopFlag) {
                    Serial.println("Critical MQTT error. Halting MQTT task");
                    vTaskSuspend(NULL);
                }
            }
            mqttClient.loop();
        }

        // Sleep until next connection check is due
        if(pdFALSE == xTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(MQTT_TASK_CYCLE_TIME_MS))) {
            Serial.println("MQTT task cycle time too low");
        }
    }
    // Task should never return
}

void mqttSetup(){
    // mqtt setup
    // If broker address is 0.0.0.0 or the string is empty, don't use MQTT
    if(strcmp(config.mqtt.broker_address, "0.0.0.0") != 0 || strlen(config.mqtt.broker_address) > 0) {
        if(mqttTaskHandle == nullptr){
            if(pdPASS !=
                xTaskCreate(mqttTask, MQTT_TASK_NAME, MQTT_TASK_STACK_SIZE, NULL, MQTT_TASK_PRIORITY, &mqttTaskHandle)) {
                 Serial.println("Failed to create MQTT task");
             }
        }
        // else the task was already created
    }
}