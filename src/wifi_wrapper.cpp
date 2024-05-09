#include "wifi_wrapper.h"
#include "globals.h"

void wifi_wrapper::init(){
    WiFi.mode(WIFI_STA);
    WiFi.setHostname("dialect_access_serial");
    if(_try_ssid_prog_connect())
        oled_handler::get_instance().draw_single_to_oled(oled_buffer(0,20,"OK, Prog"), u8g2_font_ncenB12_tf);   
        globals::internet_access = check_ping();
        return;
    if(_try_ssid_field_connect())
        oled_handler::get_instance().draw_single_to_oled(oled_buffer(0,20,"OK, Field"), u8g2_font_ncenB12_tf);
        globals::internet_access = check_ping();
        return;
}

void wifi_wrapper::process(){
    if(_ip_has_changed()){
        if(WiFi.localIP().toString().indexOf("0.0.0.0") != -1) {
            oled_handler::get_instance().draw_single_to_oled(oled_buffer(0,20,"WIFI RECONNECT..."), u8g2_font_ncenB08_tf);
            WiFi.disconnect();
            fail_count = 0;
            WiFi.begin(ssid_field, psk_field);
            while (WiFi.waitForConnectResult() != WL_CONNECTED && fail_count <= 5) {
                delay(5000);
            }
            if(WiFi.waitForConnectResult() != WL_CONNECTED) {
                oled_handler::get_instance().push_buffer(0, 10, "Reconnect Failed");
                oled_handler::get_instance().push_buffer(0, 20, "Restarting in 5sec...");
                oled_handler::get_instance().draw_to_oled(u8g2_font_ncenB08_tf);
                delay(5000);
                ESP.restart();
            }
        }  
    
    }
}
/*---PRIVATE---*/
bool wifi_wrapper::_try_ssid_prog_connect(){
    WiFi.begin(ssid_prog, psk_prog);
    while (WiFi.waitForConnectResult() != WL_CONNECTED && fail_count <= 5) {
        delay(500);
        fail_count++;
    }
    fail_count = 0;
    if(WiFi.waitForConnectResult() != WL_CONNECTED) 
        return false;
    return true;
}

bool wifi_wrapper::_try_ssid_field_connect(){
    WiFi.begin(ssid_field, psk_field);
    while (WiFi.waitForConnectResult() != WL_CONNECTED && fail_count <= 5) {
        delay(5000);
        fail_count++;
    }
    if(WiFi.waitForConnectResult() != WL_CONNECTED) {
        oled_handler::get_instance().draw_single_to_oled(oled_buffer(0,20,"FAIL, Reboot"), u8g2_font_ncenB12_tf);
        ESP.restart();
    }
    return true;
}

bool wifi_wrapper::_ip_has_changed(){
    return WiFi.localIP().toString() == "0.0.0.0";
}

/*---PRIVATE---*/