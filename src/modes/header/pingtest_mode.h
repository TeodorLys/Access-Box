#pragma once
#include "mode_base.h"
#include <ESP32Ping.h>
#include <IPAddress.h>
#include <Arduino.h>

class pingtest_mode : public mode {
private:
    bool internet_access;

public:
    void init();
    void process();
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