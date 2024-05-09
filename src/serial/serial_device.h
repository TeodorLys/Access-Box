#pragma once
#include "oled_handler.h"


class serial_device {
private:

public:
    void process(String _device){
        oled_handler::get_instance().push_buffer(0, 10, "New device connected");
        oled_handler::get_instance().push_buffer(0, 25, _device);
        oled_handler::get_instance().draw_to_oled(u8g2_font_ncenB08_tf);
        delay(1500);
    }
};