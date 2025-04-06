#include "wifi_functions.h"
#include "global.h"

const char* encryption_type_to_string(wifi_auth_mode_t encryptionType) {
    switch(encryptionType) {
        case WIFI_AUTH_OPEN:
            return "open";
        case WIFI_AUTH_WEP:
            return "WEP";
        case WIFI_AUTH_WPA_PSK:
            return "WPA";
        case WIFI_AUTH_WPA2_PSK:
            return "WPA2";
        case WIFI_AUTH_WPA_WPA2_PSK:
            return "WPA+WPA2";
        case WIFI_AUTH_WPA2_ENTERPRISE:
            return "WPA2-EAP";
        case WIFI_AUTH_WPA3_PSK:
            return "WPA3";
        case WIFI_AUTH_WPA2_WPA3_PSK:
            return "WPA2+WPA3";
        case WIFI_AUTH_WAPI_PSK:
            return "WAPI";
        default:
            return "unknown";
    }
}

uint32_t wifi_scan() {
    const uint32_t n = WiFi.scanNetworks();
    if(0 == n) {
        Serial.println("No networks found");
    } else {
        Serial.printf("%2u networks found. Listing up to 5 strongest\n", n);
        Serial.println("Nr | SSID                             | RSSI | CH | Encryption");
        for(uint32_t i = 0; i < n && i < 5; i++) {
            Serial.printf("%2u | %-32.32s | %4d | %2d | %s\n", i + 1, WiFi.SSID(i), WiFi.RSSI(i), WiFi.channel(i),
                          encryption_type_to_string(WiFi.encryptionType(i)));
        }
    }
    WiFi.scanDelete();
    return n;
}

void wifi_setup() {
    wifi_scan();

    WiFi.mode(WIFI_STA);
    WiFi.setHostname(config.wifi.hostname);
    WiFi.begin(config.wifi.ssid, config.wifi.password);
    // Generate feedback in terminal every second
    for(uint32_t i = 0; i < WIFI_CONNECTION_TIMEOUT_MS / 1000; i++) {
        // if connected, break out of wait loop
        if(WL_CONNECTED == WiFi.status()) break;
        Serial.print('.');
        delay(1000);
    }
    Serial.print("\n");

    if(WL_CONNECTED != WiFi.status()) {
        // WiFi setup failed
        
        Serial.printf("Failed to connect to %s network. Status=%u\n", config.wifi.ssid, WiFi.status());
        // WiFi.mode(WIFI_AP);
        // if(!WiFi.softAP(WIFI_AP_NAME)) {
        //     Serial.println("Failed to setup softAP");
        // }
    } else {
        // successfull connection
        Serial.printf(
            "Successful connection. IP Address: %s, "
            "DNS Server: %s\n",
            WiFi.localIP().toString().c_str(), WiFi.dnsIP().toString().c_str());
        WiFi.setAutoReconnect(true);
    }
}