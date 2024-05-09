#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "oled_handler.h"
#include <ESP32Ping.h>
#include "serial/serial_wrapper.h"
#include "serial/serial_interface.h"
#include "wifi_wrapper.h"
#include "ota.h"
#include "modes/header/mode_handler.h"
//variabls for blinking an LED with Millis
unsigned long previousMillis = 0;  // will store last time OLED Screen updated
mode_handler modes;

String db9_buffer = "";
wifi_wrapper wifi;
ota _ota;
int fail_count = 0;
serial_interface _ser_interface;

/*bool new_wifi_mode(){
  if(serial_int.get_argument("ssid") == "NaN"){
    serial.print("No SSID was given");
    serial_int.set_active_mode("default");
    return false;
  }
  if(serial_int.get_argument("psk") == "NaN"){
    serial.print("No PSK was given");
    serial_int.set_active_mode("default");
    return false;
  }
  oled.push_buffer(0, 10, "NEW WIFI MODE:");
  oled.push_buffer(0, 20, "SSID: " + serial_int.get_argument("ssid"));
  oled.push_buffer(0, 30, "Trying to connect...");
  oled.draw_to_oled(u8g2_font_ncenB08_tf);
  WiFi.disconnect();
  fail_count = 0;
  WiFi.begin(serial_int.get_argument("ssid"), serial_int.get_argument("psk"));
  while (WiFi.waitForConnectResult() != WL_CONNECTED && fail_count <= 5) {
    delay(5000);
    fail_count++;
  }
  if(WiFi.waitForConnectResult() != WL_CONNECTED) {
    serial_int.set_active_mode("default");
    oled.draw_single_to_oled(oled_buffer(0,20,"FAIL: " + serial_int.get_argument("ssid")), u8g2_font_ncenB14_tr);
    WiFi.disconnect();
    WiFi.begin(field_ssid, field_password);
    fail_count = 0;
    while (WiFi.waitForConnectResult() != WL_CONNECTED && fail_count <= 5) {
      delay(5000);
      fail_count++;
    }
    if(WiFi.waitForConnectResult() != WL_CONNECTED) {
      oled.draw_single_to_oled(oled_buffer(0,20,"REVERT FAILED, Reboot"), u8g2_font_ncenB12_tf);
      oled.push_buffer(0, 10, "REVERT FAILED");
      oled.push_buffer(0, 25, "Rebooting...");
      oled.draw_to_oled(u8g2_font_ncenB12_tf);
      ESP.restart();
    }
    serial.print("Could not connect to " + serial_int.get_argument("ssid") + ", reverted back to previous state");
    return false;
  }else {
    oled.draw_single_to_oled(oled_buffer(0,20,"OK: " + serial_int.get_argument("ssid")), u8g2_font_ncenB14_tr);
    return true;
  }
}*/
void setup() {
  Serial.begin(9600);
  Serial1.begin(115200, SERIAL_8N1, 3, 2);
  oled.begin();
  oled.draw_single_to_oled(oled_buffer(0,20,"BOOTING..."), u8g2_font_ncenB14_tr);
  wifi.init();

  _ota.init();
  if(serial.begin(WiFi.broadcastIP().toString(), 13075)){
    oled.draw_single_to_oled(oled_buffer(0,20,"SERIAL OK"), u8g2_font_ncenB12_tf);
  }else {
    oled.draw_single_to_oled(oled_buffer(0,20,"SERIAL FAIL"), u8g2_font_ncenB12_tf);
  }
  _ser_interface.set_mode_handler(&modes);
  previousMillis = millis() - 1000;
}

void loop() {
  _ota.process();
  serial.process();
  if(millis() - previousMillis >= 1000){
    wifi.process();
    modes.process();
    previousMillis = millis();
  }
  /*if(millis() - previousMillis >= 1000 && serial_int.get_active_mode() == "default"){
    String local_ip = "IP: " + WiFi.localIP().toString();
    String gw_str = "GW: " + WiFi.gatewayIP().toString();
    String info_str = "UDP: 13075,";
      if(internet_access) {
        info_str += " WAN:OK";
      }else {
        info_str += " WAN:FAIL";
      }
    oled.push_buffer(0, 10, local_ip);
    oled.push_buffer(0, 20, gw_str);
    oled.push_buffer(0, 30, info_str);
    oled.draw_to_oled(u8g2_font_ncenB08_tf);
    previousMillis = millis();
  }*/

  /*if(WiFi.localIP().toString().indexOf("0.0.0.0") != -1) {
    oled.draw_single_to_oled(oled_buffer(0,20,"WIFI RECONNECT..."), u8g2_font_ncenB08_tf);
    WiFi.disconnect();
    fail_count = 0;
    WiFi.begin(field_ssid, field_password);
    while (WiFi.waitForConnectResult() != WL_CONNECTED && fail_count <= 5) {
      delay(5000);
    }
    if(WiFi.waitForConnectResult() != WL_CONNECTED) {
      oled.push_buffer(0, 10, "Reconnect Failed");
      oled.push_buffer(0, 20, "Restarting in 5sec...");
      oled.draw_to_oled(u8g2_font_ncenB08_tf);
      delay(5000);
      ESP.restart();
    }
  }*/
  
  /*if(millis() - pingTest >= 1000 && serial_int.get_active_mode() == "pingtest") {
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
    pingTest = millis();
  }

  if(serial_int.get_active_mode() == "newwifi") {
    if(!new_wifi_mode()){
      serial.print("Fail...");
    }else {
      serial_int.set_active_mode("default");
    }
  }*/

}

