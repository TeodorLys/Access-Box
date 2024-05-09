#pragma once
#include <Arduino.h>
#include "mode_base.h"
#include "new_wifi_mode.h"
#include "pingtest_mode.h"
#include "default_mode.h"

class mode_handler {
private:
    mode *_active_mode;
    new_wifi_mode _wifi_mode;
    pingtest_mode _pingtest_mode;
    default_mode _default_mode;

public:
    mode_handler() {_active_mode = &_default_mode;}
    void set_active_mode(String _m) {
        Serial.println("Changing active mode to " + _m);
        if(_m == "newwifi")
            _active_mode = &_wifi_mode;
        else if(_m == "pingtest")
            _active_mode = &_pingtest_mode;
        else if(_m == "default")
            _active_mode = &_default_mode;
    }
    void process();
};