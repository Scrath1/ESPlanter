#include "mqtt_discovery.h"
#include <ArduinoJson.h>
#include "global.h"
#include "mqtt_functions.h"

void send_mqtt_discovery_msg(){
    JsonDocument doc_root;
    char msg[MAX_AUTO_DISCOVERY_MSG_LEN] = "";

    char efuse_mac[32] = "";
    snprintf(efuse_mac, sizeof(efuse_mac), "%llX", ESP.getEfuseMac());

    char state_topic[MAX_TOPIC_STRING_SIZE] = "";
    char cmd_topic[MAX_TOPIC_STRING_SIZE] = "";
    char unique_id[32] = "";

    JsonObject obj_device = doc_root["device"].to<JsonObject>();
    obj_device["ids"] = efuse_mac;
    obj_device["name"] = config.wifi.hostname;
    obj_device["mf"] = MANUFACTURER;
    obj_device["mdl"] = MODEL;
    obj_device["sw"] = SW_VERSION;
    obj_device["hw"] = HW_VERSION;
    obj_device["sn"] = efuse_mac;

    JsonObject obj_origin = doc_root["origin"].to<JsonObject>();
    obj_origin["name"] = MODEL;
    obj_origin["sw"] = SW_VERSION;
    obj_origin["url"] = SW_URL;

    JsonObject obj_components = doc_root["cmps"].to<JsonObject>();
    JsonObject obj_cmp_moisture = obj_components["moisture_sensor"].to<JsonObject>();
    obj_cmp_moisture["platform"] = "sensor";
    obj_cmp_moisture["device_class"] = "humidity";
    obj_cmp_moisture["unit_of_measurement"] = "%";
    snprintf(state_topic, sizeof(state_topic), "%s/%s/moisture", MQTT_BASE_TOPIC, config.mqtt.device_topic);
    obj_cmp_moisture["state_topic"] = state_topic;
    obj_cmp_moisture["value_template"] = "{{value|float}}",
    snprintf(unique_id, sizeof(unique_id), "%llx_h", ESP.getEfuseMac());
    obj_cmp_moisture["unique_id"] = unique_id;

    JsonObject obj_cmp_light = obj_components["light_sensor"].to<JsonObject>();
    obj_cmp_light["platform"] = "sensor";
    obj_cmp_light["device_class"] = "illuminance";
    obj_cmp_light["unit_of_measurement"] = "lx";
    snprintf(state_topic, sizeof(state_topic), "%s/%s/lux", MQTT_BASE_TOPIC, config.mqtt.device_topic);
    obj_cmp_light["state_topic"] = state_topic;
    obj_cmp_light["value_template"] = "{{value|int}}",
    snprintf(unique_id, sizeof(unique_id), "%llx_l", ESP.getEfuseMac());
    obj_cmp_light["unique_id"] = unique_id;

    JsonObject obj_cmp_pump_dur = obj_components["pump_duration"].to<JsonObject>();
    obj_cmp_pump_dur["platform"] = "number";
    obj_cmp_pump_dur["device_class"] = "duration";
    obj_cmp_pump_dur["unit_of_measurement"] = "ms";
    snprintf(state_topic, sizeof(state_topic), "%s/%s/pump/duration_ms", MQTT_BASE_TOPIC, config.mqtt.device_topic);
    obj_cmp_pump_dur["state_topic"] = state_topic;
    obj_cmp_pump_dur["command_topic"] = state_topic;
    obj_cmp_pump_dur["value_template"] = "{{value|int}}";
    snprintf(unique_id, sizeof(unique_id), "%llx_pd", ESP.getEfuseMac());
    obj_cmp_pump_dur["unique_id"] = unique_id;
    obj_cmp_pump_dur["min"] = 100;
    obj_cmp_pump_dur["max"] = MAX_PUMP_RUNTIME_MS;
    obj_cmp_pump_dur["name"] = "Pump active duration";

    JsonObject obj_cmp_pump_trig = obj_components["pump_trigger"].to<JsonObject>();
    obj_cmp_pump_trig["platform"] = "button";
    snprintf(cmd_topic, sizeof(cmd_topic), "%s/%s/pump/trigger", MQTT_BASE_TOPIC, config.mqtt.device_topic);
    obj_cmp_pump_trig["command_topic"] = cmd_topic;
    obj_cmp_pump_trig["payload_press"] = "1";
    snprintf(unique_id, sizeof(unique_id), "%llx_pt", ESP.getEfuseMac());
    obj_cmp_pump_trig["unique_id"] = unique_id;
    obj_cmp_pump_trig["name"] = "Pump trigger";

    serializeJson(doc_root, msg, sizeof(msg));

    char topic[MAX_TOPIC_STRING_SIZE] = "";
    snprintf(topic, sizeof(topic), "%s/device/%s/config", config.mqtt.discovery_prefix, efuse_mac);
    if(!mqttClient.publish(topic, msg, true, 1)){
        Serial.println("Failed to publish auto-discovery message");
    }
}