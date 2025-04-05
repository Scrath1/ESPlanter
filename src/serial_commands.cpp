#include "serial_commands.h"
#include "static_config.h"
#include <Arduino.h>
#include "global.h"
#include "wifi_helpers.h"

char input_buffer[SERIAL_CMD_INPUT_BUFFER_SIZE];

void print_config(const ConfigTable_t& cfg_table) {
    Serial.println("Current config:");
    for(size_t i = 0; i < cfg_table.count; i++) {
        const auto& e = cfg_table.entries[i];
        Serial.print(e.key);
        Serial.print(": ");
        if(e.perm == CFG_PERM_SECRET_RO || e.perm == CFG_PERM_SECRET_RW){
            Serial.println("<redacted>");
            continue;
        }
        switch(e.type) {
            default:
            case CONFIG_UINT32:
                Serial.print(*static_cast<uint32_t*>(e.value));
                break;
            case CONFIG_INT32:
                Serial.print(*static_cast<int32_t*>(e.value));
                break;
            case CONFIG_BOOL:
                Serial.print(*static_cast<bool*>(e.value));
                break;
            case CONFIG_FLOAT:
                Serial.print(*static_cast<float*>(e.value));
                break;
            case CONFIG_STRING:
                Serial.print(static_cast<char*>(e.value));
                break;
        }
        Serial.printf(", Max Bytes: %u\n", e.size);
    }
}

void cmd_help(const char* cmd, uint32_t cmd_len) {
    (void) cmd;
    (void) cmd_len;
    const Stint::Command * const cmd_table = stint.getCommands();
    for(uint32_t i = 0; i < stint.getNumCommands(); i++){
        const Stint::Command& cmd = cmd_table[i];
        Serial.print(cmd.name);
        Serial.print(": ");
        Serial.println(cmd.helptext);
    }
}

void cmd_list(const char* cmd, uint32_t cmd_len) {
    (void) cmd;
    (void) cmd_len;
    print_config(config_table);
}

void cmd_set(const char* cmd, uint32_t cmd_len) {
    char buf[256] = "";
    strncpy(buf, cmd, cmd_len);
    CfgRet_t ret = config_parseKVStr(&config_table, buf, cmd_len);
    switch(ret){
        default:
            Serial.printf("Unknown error while parsing: Code=%i", ret);
            break;
        case CFG_RC_SUCCESS:
            Serial.println("Updated value");
            config_saveToFile(&config_table, CONFIG_FILE_NAME);
            break;
        case CFG_RC_ERROR:
            Serial.println("Could not parse value");
            break;
        case CFG_RC_ERROR_UNKNOWN_KEY:
            Serial.println("Unknown config key");
            break;
    }
}

void cmd_get(const char* cmd, uint32_t cmd_len) {
    ConfigEntry_t entry;

    if(CFG_RC_SUCCESS != config_getByKey(&config_table, cmd, &entry)){
        Serial.println("Couldn't find matching config entry");
        return;
    }
    if(entry.perm == CFG_PERM_SECRET_RO || entry.perm == CFG_PERM_SECRET_RW){
        Serial.println("Protected config entry value");
        return;
    }
    Serial.print(entry.key);
    Serial.print(": ");
    switch(entry.type) {
        default:
        case CONFIG_UINT32:
            Serial.println(*static_cast<uint32_t*>(entry.value));
            break;
        case CONFIG_INT32:
            Serial.println(*static_cast<int32_t*>(entry.value));
            break;
        case CONFIG_BOOL:
            Serial.println(*static_cast<bool*>(entry.value));
            break;
        case CONFIG_FLOAT:
            Serial.println(*static_cast<float*>(entry.value));
            break;
        case CONFIG_STRING:
            Serial.println(static_cast<char*>(entry.value));
            break;
    }
}

void cmd_reset(const char* cmd, uint32_t cmd_len){
    (void) cmd;
    (void) cmd_len;
    ESP.restart();
}

void cmd_wifiSetup(const char* cmd, uint32_t cmd_len){
    (void) cmd;
    (void) cmd_len;
    wifiSetup();
}

Stint::Command commands[] = {
    {.name = "help", .function = cmd_help, .helptext = "Lists all commands and their helptext"},
    {.name = "list", .function = cmd_list, .helptext = "Lists all config variable names"},
    {.name = "set", .function = cmd_set, .helptext = "Sets a variable in the config by name, e.g. \"set config.wifi.ssid: ExampleSSID\""},
    {.name = "get", .function = cmd_get, .helptext = "Prints the value of a config variable"},
    {.name = "reset", .function = cmd_reset, .helptext = "Restarts the microcontroller"},
    {.name = "wifi_setup", .function = cmd_wifiSetup, .helptext = "Re-runs the WiFi setup process"}
};

Stint stint{commands, sizeof(commands) / sizeof(commands[0]), input_buffer, SERIAL_CMD_INPUT_BUFFER_SIZE};