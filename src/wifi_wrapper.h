#pragma once
#include <WiFi.h>
#include <ESPmDNS.h>
#include "oled_handler.h"
#include <ESP32Ping.h>

class wifi_wrapper {
private:
    const char *ssid_prog = "Teodors";
    const char *psk_prog = "299792458Wifi";
    const char *ssid_field = "Dialect_Access";
    const char *psk_field = "J4S-66K-tr3!";
    int fail_count = 0;

public:
    void init();
    void process();

private:
    bool _try_ssid_prog_connect();
    bool _try_ssid_field_connect();
    bool _ip_has_changed();

private:
    bool check_ping(String ip = "8.8.8.8") {
        IPAddress addr;
        addr.fromString(ip);
        if(Ping.ping(addr)) {
            return true;
        } else {
            return false;
        }
    }
};
