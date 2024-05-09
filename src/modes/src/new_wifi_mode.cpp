#include "../header/new_wifi_mode.h"
#include "../../serial/serial_wrapper.h"
#include <WiFi.h>
void new_wifi_mode::init() {
    oled.push_buffer(0, 10, "NEW WIFI MODE:");
    oled.push_buffer(0, 20, "SSID: " + _ser_interface.get_argument("ssid"));
    oled.push_buffer(0, 30, "Trying to connect...");
    oled.draw_to_oled(u8g2_font_ncenB08_tf);
}

void new_wifi_mode::process() {
    if(!mode_call_has_arguments()) 
        return;
    if(!_connect_to_new_wifi()){
        return;
    }
}

bool new_wifi_mode::mode_call_has_arguments(){
    if(_ser_interface.get_argument("ssid") == "NaN"){
        serial.print("No SSID was given");
        _ser_interface.set_active_mode("default");
        return false;
    }
    if(_ser_interface.get_argument("psk") == "NaN"){
        serial.print("No PSK was given");
        _ser_interface.set_active_mode("default");
        return false;
    }
    return true;
}

/*---PRIVATE---*/

bool new_wifi_mode::_connect_to_new_wifi(){
    WiFi.begin(_ser_interface.get_argument("ssid"), _ser_interface.get_argument("psk"));
    while (WiFi.waitForConnectResult() != WL_CONNECTED && fail_count <= 5) {
        delay(5000);
        fail_count++;
    }
    if(WiFi.waitForConnectResult() != WL_CONNECTED) {
        _ser_interface.set_active_mode("default");
        oled.draw_single_to_oled(oled_buffer(0,20,"FAIL: " + _ser_interface.get_argument("ssid")), u8g2_font_ncenB14_tr);
        WiFi.disconnect(); // Let the wifi_wrapper do the reconnect to the field wifi
        return false;
    }
    oled.draw_single_to_oled(oled_buffer(0,20,"OK: " + _ser_interface.get_argument("ssid")), u8g2_font_ncenB14_tr);
    return true;
}

/*---PRIVATE---*/