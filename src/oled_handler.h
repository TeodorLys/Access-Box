#pragma once
#include <U8g2lib.h>
#include <Vector.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
struct oled_buffer {
    u8g2_uint_t x, y;
    String str;
    bool draw = false;
    oled_buffer(u8g2_uint_t _x, u8g2_uint_t _y, String _str) : x(_x), y(_y), str(_str) { draw = true; }
    oled_buffer(){}
};



U8G2_SSD1306_128X32_UNIVISION_1_SW_I2C u8g2(U8G2_R0, /* clock=*/9, /* data=*/8, /* reset=*/U8X8_PIN_NONE);

class oled_handler {
private:
    oled_buffer draw_buffer[10];

public:
    oled_handler() {}

    void begin() {
        u8g2.begin();
        u8g2.enableUTF8Print();	
    }
    void push_buffer(u8g2_uint_t x, u8g2_uint_t y, String str){
        //draw_buffer.push_back(oled_buffer(x, y, str));
        for (int a = 0; a < 10; a++){
            if(draw_buffer[a].draw == false){
                draw_buffer[a].x = x;
                draw_buffer[a].y = y;
                draw_buffer[a].str = str;
                draw_buffer[a].draw = true;
                break;
            }
        }
    }

    void draw_to_oled(const uint8_t font[] = u8g2_font_ncenB12_tr){
        u8g2.setFont(font);
        u8g2.firstPage();
        do {
            for (int a = 0; a < 10; a++) {
                if(draw_buffer[a].draw == true){
                    u8g2.setCursor(draw_buffer[a].x, draw_buffer[a].y);
                    u8g2.print(draw_buffer[a].str);
                }
            }
        } while ( u8g2.nextPage() );
        clear_draw_buffer();
    }
    void draw_single_to_oled(oled_buffer buff, const uint8_t font[] = u8g2_font_ncenB12_tr){
        Serial.printf("%i, %i, %s\n", buff.x, buff.y, buff.str.c_str());
        u8g2.setFont(font);
        u8g2.firstPage();
        do {
            u8g2.setCursor(buff.x, buff.y);
            u8g2.print(buff.str);
        } while ( u8g2.nextPage() );
    }
    U8G2_SSD1306_128X32_UNIVISION_1_SW_I2C& get_raw_var() { return u8g2; }

private:
    void clear_draw_buffer(){
        for (int a = 0; a < 10; a++) {
            draw_buffer[a].draw = false;
        }
    }
};