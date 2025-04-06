#include "mqtt_functions.h"
#include "drivers/pump.h"

WiFiClient espWiFiClient;
PubSubClient mqttClient(espWiFiClient);
// The topic on which messages to trigger the pump are received
char pump_trigger_topic[256] = "";
bool stop_connection_attempts = false;

void update_mqtt_subscriptions(){
    if(!mqttClient.unsubscribe(pump_trigger_topic)){
        Serial.println("Failed to unsubscribe from old pump topic");
    }
    snprintf(pump_trigger_topic, sizeof(pump_trigger_topic), "%s/%s/trigger_pump_ms", MQTT_BASE_TOPIC,
                 config.mqtt.device_topic);
    if(!mqttClient.subscribe(pump_trigger_topic)){
        Serial.println("Failed to subscribe to new pump topic");
    }
}

void mqttMaintainConnection(){
    static bool last_mqtt_connection_status = false;
    if(!WiFi.isConnected()) return;
    // If the connection was lost, reconnect
    if(!mqttClient.connected() && !stop_connection_attempts) {
        // lost connection. Try to reconnect...
        int8_t err = mqttClient.connect(config.mqtt.client_id, config.mqtt.username, config.mqtt.password);
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
                stop_connection_attempts = true;
                break;
            case MQTT_CONNECT_UNAUTHORIZED:
                Serial.println("MQTT client not authorized");
                stop_connection_attempts = true;
                break;
        }
    }
    // Check whether a connection was just established
    if(!last_mqtt_connection_status && mqttClient.connected()){
        Serial.println("Connected to MQTT server");
        update_mqtt_subscriptions();
        Serial.printf("Pump trigger topic is: %s\n", pump_trigger_topic);
        if(!mqttClient.publish(pump_trigger_topic, "0")){
            Serial.println("Failed to send message to pump trigger topic");
        }
        // ToDo: Send MQTT discovery message
    }
    last_mqtt_connection_status = mqttClient.connected();
}

void mqtt_subscription_callback(const char topic[], byte* payload, unsigned int length) {
    Serial.printf("Received MQTT message: topic=%s, payload=%s",topic, payload);
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
        run_pump(duration_ms);
    }
}

void mqttSetup() {
    // mqtt setup
    // If broker address is 0.0.0.0 or the string is empty, don't use MQTT
    if(strcmp(config.mqtt.broker_address, "0.0.0.0") != 0 || strlen(config.mqtt.broker_address) > 0) {
        mqttClient.setKeepAlive(MQTT_CONNECTION_KEEPALIVE_S);
        IPAddress serverIP;
        serverIP.fromString(config.mqtt.broker_address);
        Serial.printf("MQTT broker address: %s\n", serverIP.toString().c_str());
        mqttClient.setServer(serverIP, config.mqtt.broker_port);

        // else the task was already created
        mqttClient.setCallback(mqtt_subscription_callback);
    }
}