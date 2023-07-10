#pragma once
#include <Arduino.h>
#include <WiFiUdp.h>
#include "serial_interface.h"
#include "oled_handler.h"
class serial_wrapper {
private:
    WiFiUDP udp;
    int port = -1;
    String broadcast_ip = "0.0.0.0";
    serial_interface *_ser_int;
public: 
    serial_wrapper(serial_interface *ser_int) : _ser_int(ser_int) {}
    serial_wrapper() {}
    serial_wrapper(String broadcast, int p) {
        begin(broadcast, p);
    }
    
    bool begin(String broadcast, int p){
        broadcast_ip = broadcast;
        port = p;
        return udp.begin(port);
    }

    void print(String str){
        if(str == "\r"){
            //Serial1.println(str);
            write_to_udp_port(str);    
        }else {
            //Serial1.print(str);
            write_to_udp_port(str);
        }
        
    }
    void println(String str){
        //Serial1.println(str);
        write_to_udp_port(str);
    }
    int available(){
        return udp.parsePacket();
    }
    void read_and_print(){
        //Serial1.print("reading and printint the udp packet");
        char packetBuffer[64];
        int r_size = udp.read(packetBuffer, 64);
        String packet = String(packetBuffer, r_size);
        if( _ser_int->get_prog_mode_state() == true){
            write_to_udp_port(_ser_int->parse_packet(packet));
            return;
        }
        if(packet.indexOf("enter prog mode") != -1){
            _ser_int->set_prog_mode_active();
            write_to_udp_port("Programming mode active!");
        }else {
            Serial1.print(packet);
            write_to_udp_port(packet);
        }

    }
private:
    void write_to_udp_port(String str){ 
        IPAddress address;
        address.fromString(broadcast_ip);
        udp.beginPacket(address, port);
        udp.print(str);
        udp.endPacket();
    }
};