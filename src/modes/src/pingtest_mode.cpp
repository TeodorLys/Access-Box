#include "../header/pingtest_mode.h"
#include <Arduino.h>
#include "../../oled_handler.h"
#include "../../serial/serial_interface.h"
#include <U8g2lib.h>

void pingtest_mode::init() {

}

void pingtest_mode::process() {
    String mode = "PING MODE";
    String ping_test_ip = "";
    if(serial_int.get_argument("ip") != "NaN") {
      internet_access = check_ping(serial_int.get_argument("ip"));
      ping_test_ip = serial_int.get_argument("ip") + ", " + String((int)Ping.averageTime()) + "ms";
    }else {
      internet_access = check_ping();
      ping_test_ip = "8.8.8.8, " + String((int)Ping.averageTime()) + "ms";
    }
    String info_str;
    if (internet_access) {
      info_str += "OK, ";
    }else {
      info_str += "FAIL, ";
    }
    info_str += WiFi.localIP().toString();
    oled.push_buffer(0, 10, mode);
    oled.push_buffer(0, 20, ping_test_ip);
    oled.push_buffer(0, 30, info_str);
    oled.draw_to_oled(u8g2_font_ncenB08_tf);
}
