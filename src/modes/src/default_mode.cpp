#include "../header/default_mode.h"
#include <Arduino.h>
#include <U8g2lib.h>
#include <WiFi.h>
#include "../../oled_handler.h"
#include "../../globals.h"

void default_mode::init(){

}

void default_mode::process() {    
    String local_ip = "IP: " + WiFi.localIP().toString();
    String gw_str = "GW: " + WiFi.gatewayIP().toString();
    String info_str = "UDP: 13075,";
    if(globals::internet_access) {
      info_str += " WAN:OK";
    }else {
      info_str += " WAN:FAIL";
    }
    oled.push_buffer(0, 10, local_ip);
    oled.push_buffer(0, 20, gw_str);
    oled.push_buffer(0, 30, info_str);
    oled.draw_to_oled(u8g2_font_ncenB08_tf);
}