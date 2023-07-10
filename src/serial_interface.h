#pragma once
#include <Arduino.h>
struct dict {
    String name = "";
    String value = "";
    bool ok = false;
};
class serial_interface {
private:
    bool prog_mode_state;
    String active_mode = "default";
    dict arg_buffer[10];
    String raw_req = "";
    String msg = "";

public:
    String parse_packet(String str){
        if(!str.endsWith(" ")){
            str += " ";
        }
        if(str.startsWith("mode")) {
            raw_req = str;
            str.replace("mode ", "");
            active_mode = str.substring(0, str.indexOf(" "));
            str.replace(active_mode + " ","");
            msg = str;
            if(str.length() > 2) {
                if(!_argument_parse_helper(str)){
                    active_mode = "default";
                    return "FAILED: could not parse arguments";
                }
            }
            return "OK";
        }else {
            return "FAILED: Packet did not start with 'mode'";
        }
    }
    String get_argument(String name){
        for (int i = 0; i < 10; i++){
            if(arg_buffer[i].name == name){
                return arg_buffer[i].value;
            }
        }
        return "NaN";
    }
    String get_args() {
        String tmp = raw_req + "," + msg + "  -  ";
        for(int i = 0; i < 2; i++){
            tmp += arg_buffer[i].name + ", " + arg_buffer[i].value + " - ";
        }
        return tmp;
    }
    String get_active_mode() { return active_mode; }
    void set_prog_mode_active(){ prog_mode_state = true; }
    bool get_prog_mode_state() const { return prog_mode_state; }
private:
    bool _argument_parse_helper(String arg){
        raw_req = arg;
        int count = _count_args(arg, '-');
        for(int i = 0; i < count; i++){
            String arg_name = arg.substring(0, arg.indexOf(' '));
            if(!arg_name.startsWith("-"))
                return false;
            arg.replace(arg_name + " ", "");
            if(arg.length() < 2)
                return false;
            String arg_value = arg.substring(0, arg.indexOf(' '));
            arg_name.replace("-", "");
            arg_buffer[i].name = arg_name;
            arg_buffer[i].value = arg_value;
        }
        return true;
    }
    
    int _count_args(String arg, char key){
        int count = 0;
        for (int a = 0;a < arg.length(); a++){
            if(arg[a] == key) count++;
        }
        return count;
    }

};
