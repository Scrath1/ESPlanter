#include <Arduino.h>
#include <LittleFS.h>
#include "drivers/pump.h"
#include "global.h"
#include "static_config.h"

#define FORMAT_LITTLEFS_IF_FAILED true

CfgRet_t config_littleFSSaveToFile(const ConfigTable_t* cfg, const char* filename){
    if(cfg == NULL || filename == NULL) return CFG_RC_ERROR_NULLPTR;
    fs::File file = LittleFS.open(filename, FILE_WRITE);
    if(!file) return CFG_RC_ERROR;
    char line[FILE_MAX_LINE_LEN] = "";
    // iterate over all config entries
    bool line_length_error = false;
    bool encoding_error = false;
    for(uint32_t i = 0; i < cfg->count; i++) {
        const ConfigEntry_t e = cfg->entries[i];
        int32_t ret;
        switch(e.type) {
            default:
            case CONFIG_NONE:
                    continue;
            case CONFIG_BOOL:
                ret = snprintf(line, sizeof(line), "%s: %u\n", e.key, *(bool*)e.value);
                break;
            case CONFIG_UINT32:
                ret = snprintf(line, sizeof(line), "%s: %lu\n", e.key, *(uint32_t*)e.value);
                break;
            case CONFIG_INT32:
                ret = snprintf(line, sizeof(line), "%s: %i\n", e.key, *(int32_t*)e.value);
                break;
            case CONFIG_FLOAT:
                ret = snprintf(line, sizeof(line), "%s: %f\n", e.key, *(float*)e.value);
                break;
            case CONFIG_STRING:
                ret = snprintf(line, sizeof(line), "%s: %s\n", e.key, e.value);
                break;
        }
        // Check if snprintf was successful
        if(ret > sizeof(line)) line_length_error = true;
        else if(ret < 0) encoding_error = true;
        else {
            // write to file
            file.print(line);
        }
    }
    // close file
    file.close();

    if(encoding_error) return CFG_RC_ERROR_INVALID;
    if(line_length_error) return CFG_RC_ERROR_INCOMPLETE;
    return CFG_RC_SUCCESS;
}

CfgRet_t config_littleFSloadFromFile(ConfigTable_t* cfg, const char* filename){
    if(cfg == NULL || filename == NULL) return CFG_RC_ERROR_NULLPTR;
    fs::File file = LittleFS.open(filename, FILE_READ);
    if(!file) return CFG_RC_ERROR;

    String lineStr = "";
    char line[FILE_MAX_LINE_LEN] = "";
    bool parsing_error_occurred = false;
    // Read each line
    while(file.available()){
        lineStr = file.readStringUntil('\n');
        uint32_t line_len = lineStr.length();
        if(line_len == 0) continue;

        // copy into c string for compatibility with library functions
        strncpy(line, lineStr.c_str(), lineStr.length() + 1);
        if(CFG_RC_SUCCESS != config_parseKVStr(cfg, line, line_len + 1)) {
            // If any line could not be matched to a valid entry, set a flag
            parsing_error_occurred = true;
        }
        // reset line
        line[0] = '\0';
    }
    file.close();

    if(parsing_error_occurred) return CFG_RC_ERROR_INCOMPLETE;
    return CFG_RC_SUCCESS;
}

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
                Serial.println(*static_cast<uint32_t*>(e.value));
                break;
            case CONFIG_INT32:
                Serial.println(*static_cast<int32_t*>(e.value));
                break;
            case CONFIG_BOOL:
                Serial.println(*static_cast<bool*>(e.value));
                break;
            case CONFIG_FLOAT:
                Serial.println(*static_cast<float*>(e.value));
                break;
            case CONFIG_STRING:
                Serial.println(static_cast<char*>(e.value));
                break;
        }
    }
}

void setup() {
    delay(1000);
    Serial.begin(115200);
    Serial.println("ESPlanter");
    // put your setup code here, to run once:
    init_pump();
    if(!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED)){
        Serial.println("Failed to initialize filesystem");
        while(1);
    }
    config_setSaveLoadFunctions(config_littleFSSaveToFile, config_littleFSloadFromFile);
    if(!LittleFS.exists(CONFIG_FILE_NAME)){
        if(CFG_RC_SUCCESS != config_saveToFile(&config, CONFIG_FILE_NAME)){
            Serial.println("Failed to create default config file");
            while(1);
        }
        else{
            Serial.println("Created default config file");
        }
        
    }
    else{
        if(CFG_RC_SUCCESS != config_loadFromFile(&config, CONFIG_FILE_NAME)){
            Serial.println("Failed to load config from file");
            while(1);
        }
        else{
            Serial.println("Loaded config from file");
            print_config(config);
        }
    }
    

    Serial.println("Setup complete");
}

void loop() {
    static uint32_t rx_buffer_next_idx = 0;
    static char rx_buffer[256] = "";

    // put your main code here, to run repeatedly:
    while(Serial.available() > 0){
        char c = Serial.read();
        Serial.print(c);
        if(c == '\n' || c == '\r'){
            if(CFG_RC_SUCCESS != config_parseKVStr(&config, rx_buffer, rx_buffer_next_idx)){
                Serial.println("Failed to parse key-value pair for settings");
            }
            else{
                Serial.println("Updated config value");
                if(CFG_RC_SUCCESS != config_saveToFile(&config, CONFIG_FILE_NAME)){
                    Serial.println("Failed to save updated config to filesystem");
                }
            }
            // reset buffer
            memset(rx_buffer, '\0', rx_buffer_next_idx);
            rx_buffer_next_idx = 0;
        }
        else{
            if(rx_buffer_next_idx < sizeof(rx_buffer)){
                rx_buffer[rx_buffer_next_idx++] = c;
            }
            else{
                Serial.println("Error: Input buffer full");
            }
        }
    }
}
