#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "oled_handler.h"
#include <ESP32Ping.h>
#include "serial_wrapper.h"
#include "serial_interface.h"

const char* field_ssid = "Dialect_Access";
const char* field_password = "J4S-66K-tr3!";
const char* pref_ssid = "Teodors";
const char* pref_password = "299792458Wifi";
char extra = 'T';
//variabls for blinking an LED with Millis
const int led = 2; // ESP32 Pin to which onboard LED is connected
unsigned long previousMillis = 0;  // will store last time OLED Screen updated
unsigned long pingTest = 0;  // Last ping test
const long interval = 1000;  // interval at which to blink (milliseconds)
int ledState = LOW;  // ledState used to set the LED
bool internet_access = false; // Ping result of internet access

bool flip = false;
String db9_buffer = "";
serial_interface serial_int;
oled_handler oled;
serial_wrapper serial(&serial_int);
int fail_count = 0;
int last_prog = -1;

bool check_ping(String ip = "8.8.8.8") {
  IPAddress addr;
  addr.fromString(ip);
  if(Ping.ping(addr)) {
    return true;
  } else {
    return false;
  }
}

void setup() {
  Serial1.begin(115200, SERIAL_8N1, 3, 2);
  oled.begin();
  oled.draw_single_to_oled(oled_buffer(0,20,"BOOTING..."), u8g2_font_ncenB14_tr);
  WiFi.mode(WIFI_STA);
  WiFi.setHostname("dialect_access_serial");
  WiFi.begin(pref_ssid, pref_password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED && fail_count <= 5) {
    //Serial1.println("Connection Failed! Retrying in 5sec...");
    delay(5000);
    fail_count++;
  }
  fail_count = 0;
  
  if(WiFi.waitForConnectResult() != WL_CONNECTED) {
    oled.push_buffer(0, 10, "Waiting for AP");
    oled.push_buffer(0, 20, "15seconds...");
    oled.draw_to_oled(u8g2_font_ncenB12_tf);
    delay(15000);
    WiFi.begin(field_ssid, field_password);
    while (WiFi.waitForConnectResult() != WL_CONNECTED && fail_count <= 5) {
      //Serial1.println("Connection Failed! Retrying in 5sec...");
      delay(5000);
    }
    if(WiFi.waitForConnectResult() != WL_CONNECTED) {
      //Serial1.println("Could not connect to any network, Rebooting...");
      oled.draw_single_to_oled(oled_buffer(0,20,"FAIL, Reboot"), u8g2_font_ncenB12_tf);
      ESP.restart();
    }
  }
  if(WiFi.SSID() == pref_ssid) {
    oled.draw_single_to_oled(oled_buffer(0,20,"OK, Con-Pref"), u8g2_font_ncenB12_tf);
  }else {
    oled.draw_single_to_oled(oled_buffer(0,20,"OK, Con-Field"), u8g2_font_ncenB12_tf);
  }

  ArduinoOTA.setHostname("dialect_access_serial");

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      oled.draw_single_to_oled(oled_buffer(0,20,"Start updating " + type), u8g2_font_ncenB12_tf);
    })
    .onEnd([]() {
      oled.draw_single_to_oled(oled_buffer(0,20,"End"), u8g2_font_ncenB12_tf);
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      static int last_progress = -1;
      if((progress / (total / 100)) % 10 == 0 && last_progress != (progress / (total / 100))){
        oled.get_raw_var().setFont(u8g2_font_ncenB12_tf);
        oled.get_raw_var().firstPage();
        do {
          oled.get_raw_var().setCursor(0, 20);
          oled.get_raw_var().printf("Progress: %u%%\r", (progress / (total / 100)));
        } while ( oled.get_raw_var().nextPage() );
        last_progress = (progress / (total / 100));
      }
    })
    .onError([](ota_error_t error) {
      ///oled.draw_single_to_oled(oled_buffer(0,20,"Start updating " + type), u8g2_font_ncenB12_tf);
      oled.get_raw_var().setFont(u8g2_font_ncenB12_tf);
      oled.get_raw_var().firstPage();
      do {
        oled.get_raw_var().setCursor(0, 20);
        oled.get_raw_var().printf("Error[%u]: ", error);
      } while ( oled.get_raw_var().nextPage() );
      if (error == OTA_AUTH_ERROR){
        oled.draw_single_to_oled(oled_buffer(0,20,"AUTH ERROR"), u8g2_font_ncenB12_tf);
      } else if (error == OTA_BEGIN_ERROR) {
        oled.draw_single_to_oled(oled_buffer(0,20,"Begin Failed"), u8g2_font_ncenB12_tf);
      }
      else if (error == OTA_CONNECT_ERROR) {
        oled.draw_single_to_oled(oled_buffer(0,20,"Connect Failed"), u8g2_font_ncenB12_tf);
      }
      else if (error == OTA_RECEIVE_ERROR) {
        oled.draw_single_to_oled(oled_buffer(0,20,"Receive Failed"), u8g2_font_ncenB12_tf);
      }
      else if (error == OTA_END_ERROR) {
        oled.draw_single_to_oled(oled_buffer(0,20,"End Failed"), u8g2_font_ncenB12_tf);
      }
    });
  ArduinoOTA.begin();

  Serial1.println("Ready");
  Serial1.print("IP address: ");
  Serial1.println(WiFi.localIP());
  if(serial.begin(WiFi.broadcastIP().toString(), 13075)){
    oled.draw_single_to_oled(oled_buffer(0,20,"UDP OK"), u8g2_font_ncenB12_tf);
  }else {
    oled.draw_single_to_oled(oled_buffer(0,20,"UDP FAIL"), u8g2_font_ncenB12_tf);
  }
  internet_access = check_ping();
  previousMillis = millis() - 1000;
  pingTest = millis();
}

void loop() {
  ArduinoOTA.handle();
  //Serial1.println("Testing...");
  if(Serial1.available() > 0){
    //serial.println("Got serial response...");
    char c = Serial1.read();
    if(c != '\r' || c != '\n'){
      db9_buffer += c;
    }else {
      serial.print(db9_buffer);
      db9_buffer = "";
    }
  }else{
    if(!db9_buffer.isEmpty()){
      serial.print(db9_buffer);
      db9_buffer = "";
    }
  }

  if(serial.available() > 0){
    serial.read_and_print();
  }

  if(millis() - previousMillis >= 1000 && serial_int.get_active_mode() == "default"){
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
  }

  if(WiFi.localIP().toString() == "0.0.0.0") {
    WiFi.reconnect();
  }
  
  if(millis() - pingTest >= 1000 && serial_int.get_active_mode() == "pingtest") {
    internet_access = check_ping();
    String mode = "PING MODE";
    String ping_test_ip = "";
    if(serial_int.get_argument("ip") != "NaN") {
      ping_test_ip = serial_int.get_argument("ip") + ", " + String((int)Ping.averageTime()) + "ms";
    }else {
      ping_test_ip = "8.8.8.8, " + String((int)Ping.averageTime()) + "ms";
    }
    
    String info_str;
    if (internet_access) {
      info_str += "WAN:OK";
    }else {
      info_str += "WAN:FAIL";
    }
    oled.push_buffer(0, 10, mode);
    oled.push_buffer(0, 20, ping_test_ip);
    oled.push_buffer(0, 30, info_str);
    oled.draw_to_oled(u8g2_font_ncenB08_tf);
    pingTest = millis();
  }
}


