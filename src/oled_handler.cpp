#include "oled_handler.h"


U8G2_SSD1306_128X32_UNIVISION_1_SW_I2C oled_handler::u8g2(U8G2_R0, /* clock=*/9, /* data=*/8, /* reset=*/U8X8_PIN_NONE);
oled_buffer oled_handler::draw_buffer[10];
