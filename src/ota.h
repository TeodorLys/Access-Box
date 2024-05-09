#pragma once
#include <ArduinoOTA.h>
#include "oled_handler.h"
class ota {
private:

public:
    void init(){
        ArduinoOTA.setHostname("dialect_access_serial");
        ArduinoOTA
        .onStart([]() {
        String type;  
        if (ArduinoOTA.getCommand() == U_FLASH)
            type = "sketch";
        else // U_SPIFFS
            type = "filesystem";

        oled_handler::get_instance().draw_single_to_oled(oled_buffer(0,20,"Start updating " + type), u8g2_font_ncenB12_tf);
        })
        .onEnd([]() {
        oled_handler::get_instance().draw_single_to_oled(oled_buffer(0,20,"End"), u8g2_font_ncenB12_tf);
        })
        .onProgress([](unsigned int progress, unsigned int total) {
        static int last_progress = -1;
        if((progress / (total / 100)) % 10 == 0 && last_progress != (progress / (total / 100))){
            oled_handler::get_instance().get_raw_var().setFont(u8g2_font_ncenB12_tf);
            oled_handler::get_instance().get_raw_var().firstPage();
            do {
            oled_handler::get_instance().get_raw_var().setCursor(0, 20);
            oled_handler::get_instance().get_raw_var().printf("Progress: %u%%\r", (progress / (total / 100)));
            } while ( oled_handler::get_instance().get_raw_var().nextPage() );
            last_progress = (progress / (total / 100));
        }
        })
        .onError([](ota_error_t error) {
        oled_handler::get_instance().get_raw_var().setFont(u8g2_font_ncenB12_tf);
        oled_handler::get_instance().get_raw_var().firstPage();
        do {
            oled_handler::get_instance().get_raw_var().setCursor(0, 20);
            oled_handler::get_instance().get_raw_var().printf("Error[%u]: ", error);
        } while ( oled_handler::get_instance().get_raw_var().nextPage() );
        if (error == OTA_AUTH_ERROR){
            oled_handler::get_instance().draw_single_to_oled(oled_buffer(0,20,"AUTH ERROR"), u8g2_font_ncenB12_tf);
        } else if (error == OTA_BEGIN_ERROR) {
            oled_handler::get_instance().draw_single_to_oled(oled_buffer(0,20,"Begin Failed"), u8g2_font_ncenB12_tf);
        }
        else if (error == OTA_CONNECT_ERROR) {
            oled_handler::get_instance().draw_single_to_oled(oled_buffer(0,20,"Connect Failed"), u8g2_font_ncenB12_tf);
        }
        else if (error == OTA_RECEIVE_ERROR) {
            oled_handler::get_instance().draw_single_to_oled(oled_buffer(0,20,"Receive Failed"), u8g2_font_ncenB12_tf);
        }
        else if (error == OTA_END_ERROR) {
            oled_handler::get_instance().draw_single_to_oled(oled_buffer(0,20,"End Failed"), u8g2_font_ncenB12_tf);
        }
        });
        ArduinoOTA.begin();
    }
    void process(){
        ArduinoOTA.handle();
    }
};