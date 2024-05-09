#pragma once
#include "mode_base.h"
#include "../../serial/serial_interface.h"

class new_wifi_mode : public mode{
private:
    int fail_count = 0;
    serial_interface _ser_interface;

public:
    void init();
    void process();
    bool mode_call_has_arguments();

private:
    bool _connect_to_new_wifi();
};
