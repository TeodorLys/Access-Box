#pragma once
#include <Arduino.h>
#include <WiFiUdp.h>
#include "serial_interface.h"
#include "oled_handler.h"
#include "serial_device.h"
class serial_wrapper {
private:
    static WiFiUDP udp;
    static int port;
    static String broadcast_ip;
    bool new_device = false;
    String connected_device = "";
    serial_device _ser_device;
    serial_interface _ser_interface;

public: 
    serial_wrapper() {}
    serial_wrapper(String broadcast, int p) {
        begin(broadcast, p);
    }
    static serial_wrapper get_instance() {
        static serial_wrapper instance;
        return instance; 
    }
    
    bool begin(String broadcast, int p){
        serial_wrapper::broadcast_ip = broadcast;
        port = p;
        return udp.begin(port);
    }

    void process(){
        _hw_serial_check_available();
        if(available() > 0){
            read_and_print();
        }
    }

    void print(String str){
        _write_to_udp_port(str);    
    }
    void println(String str){
        _write_to_udp_port(str + "\n");
    }
    int available(){
        return udp.parsePacket();
    }
    void read_and_print(){
        //Serial1.print("reading and printint the udp packet");
        char packetBuffer[64];
        int r_size = udp.read(packetBuffer, 64);
        String packet = String(packetBuffer, r_size);
        _handle_receive_udp_packet(packet);
    }
    
    bool new_device_connected(){
        bool ret = new_device;
        new_device = false;
        return ret;
    }

    String get_connected_device(){
        return connected_device;
    }
private:
    void _write_to_udp_port(String str){ 
        IPAddress address;
        address.fromString(broadcast_ip);
        udp.beginPacket(address, port);
        udp.print(str);
        udp.endPacket();
    }
    void _handle_receive_udp_packet(String packet){
        _write_to_udp_port(String(_ser_interface.get_prog_mode_state()));
        if(packet.indexOf("init dialectJ4 ") != -1){
            serial_wrapper::broadcast_ip = udp.remoteIP().toString();
            packet.replace("init dialectJ4 ","");
            connected_device = packet;
            _ser_device.process(connected_device);
            new_device = true;
            _write_to_udp_port(serial_wrapper::broadcast_ip + ", OK");
            return;
        }
        if(packet.indexOf("exit prog mode") != -1){
            _ser_interface.set_prog_mode_deactive();
            _write_to_udp_port("Programming mode deactivated!");
            return;
        }
        if(_ser_interface.get_prog_mode_state() == true){
            _write_to_udp_port(packet);
            _write_to_udp_port(_ser_interface.parse_packet(packet));
            return;
        }
        if(packet.indexOf("enter prog mode") != -1){
            _ser_interface.set_prog_mode_active();
            _write_to_udp_port(String(_ser_interface.get_prog_mode_state()));
            _write_to_udp_port("Programming mode activated!");
        }else {
            Serial1.print(packet);
        }
    }
    void _hw_serial_check_available(){
        String db9_buffer = "";
        if(Serial1.available() > 0){
            char c = Serial1.read();
            if(c != '\r' || c != '\n'){
                db9_buffer += c;
            }else {
                print(db9_buffer);
                db9_buffer = "";
            }
        }else{
            if(!db9_buffer.isEmpty()){
                print(db9_buffer);
                db9_buffer = "";
            }
        }
    }
};


#define serial serial_wrapper::get_instance()