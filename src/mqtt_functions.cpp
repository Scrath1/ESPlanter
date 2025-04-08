#include "mqtt_functions.h"

#include "drivers/pump.h"

WiFiClient wifiClient;
MQTTClient mqttClient;
// The topic on which messages to trigger the pump are received
char pump_trigger_topic[256] = "";

void update_mqtt_subscriptions() {
    if(!mqttClient.unsubscribe(pump_trigger_topic)) {
        Serial.println("Failed to unsubscribe from old pump topic");
    }
    snprintf(pump_trigger_topic, sizeof(pump_trigger_topic), "%s/%s/trigger_pump_ms", MQTT_BASE_TOPIC,
             config.mqtt.device_topic);
    if(!mqttClient.subscribe(pump_trigger_topic)) {
        Serial.println("Failed to subscribe to new pump topic");
    }
}

void mqtt_on_reconnect() {
    update_mqtt_subscriptions();
    Serial.printf("Pump trigger topic is: %s\n", pump_trigger_topic);
    if(!mqttClient.publish(pump_trigger_topic, "0")) {
        Serial.println("Failed to send message to pump trigger topic");
    }
}

void mqtt_maintain_connection() {
    static uint32_t next_connection_attempt_ticks = 0;
    if(!WiFi.isConnected()) return;
    // If the connection was lost, reconnect
    if(!mqttClient.connected() && (next_connection_attempt_ticks < xTaskGetTickCount())) {
        // lost connection. Try to reconnect...
        if(!mqttClient.connect(config.mqtt.client_id, config.mqtt.username, config.mqtt.password)) {
            Serial.println("Failed to connect to MQTT server");
            // ToDo: Maybe use the low level debug lastError and returnCode for lwmqtt debug infos
            next_connection_attempt_ticks =
                xTaskGetTickCount() + pdMS_TO_TICKS(MQTT_CONNECTION_RETRY_INTERVAL_S * 1000);
        } else {
            // just connected to server
            Serial.println("Connected to MQTT server");
            mqtt_on_reconnect();
        }
    }
}

void mqtt_subscription_callback(MQTTClient* client, const char topic[], char* payload, int length) {
    Serial.printf("Received MQTT message: topic=%s, payload=%s\n", topic, payload);
    // ToDo: Subscribe to relevant topics and handle incoming MQTT messages
    if(strcmp(topic, pump_trigger_topic) == 0) {
        // pump message
        if(payload[0] == '-') return;
        uint32_t duration_ms = strtoull((char*)payload, NULL, 10);
        if(errno == ERANGE) {
            errno = 0;
            return;
        }
        // apply upper duration limit
        if(duration_ms > MAX_PUMP_RUNTIME_MS) duration_ms = MAX_PUMP_RUNTIME_MS;
        Serial.printf("Pump triggered for %lu ms\n", duration_ms);
        pump_run(duration_ms);
    }
}

void mqtt_setup() {
    // mqtt setup
    // If broker address is 0.0.0.0 or the string is empty, don't use MQTT
    if(strcmp(config.mqtt.broker_address, "0.0.0.0") != 0 || strlen(config.mqtt.broker_address) > 0) {
        mqttClient.begin(config.mqtt.broker_address, config.mqtt.broker_port, wifiClient);
        mqttClient.onMessageAdvanced(mqtt_subscription_callback);
        mqttClient.setKeepAlive(MQTT_CONNECTION_KEEPALIVE_S);
    }
}