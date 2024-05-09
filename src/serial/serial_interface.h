#pragma once
#include <Arduino.h>
#include "../modes/header/mode_handler.h"
struct dict {
    String name = "";
    String value = "";
};
class serial_interface {
private:
    static bool prog_mode_state;
    String active_mode = "default";
    static dict arg_buffer[10];
    String raw_req = "";
    String msg = "";
    String debug_msg = "";
    static mode_handler *_handler;

public:
    void set_mode_handler(mode_handler *handler) {
        _handler = handler;
    }

    String parse_packet(String str){
        if(!str.endsWith(" ")){
            str += " ";
        }
        if(str.startsWith("mode")) {
            raw_req = str;
            str.replace("mode ", "");
            active_mode = str.substring(0, str.indexOf(" "));
            _handler->set_active_mode(active_mode);
            str.replace(active_mode + " ","");
            msg = str;
            if(str.length() > 2) {
                if(!_argument_parse_helper(str)){
                    active_mode = "default";
                    return "FAILED: could not parse arguments, " + debug_msg;
                }
            }
            return "OK " + debug_msg;
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
    String get_active_mode() const { return active_mode; }
    void set_active_mode(String mode){ active_mode = mode; }
    void set_prog_mode_active(){ prog_mode_state = true; }
    void set_prog_mode_deactive(){ prog_mode_state = false; }
    bool get_prog_mode_state() { return prog_mode_state; }
private:
    bool _argument_parse_helper(String arg){
        int count = _count_spaces(arg);
        String *args = _split(arg);
        if(!_check_variable_to_value_ratio(args, count)){
            debug_msg = "Variable to value ratio is off, c:" + String(count);
            return false;
        }
        if(!_check_argument_trail_structure(args,count)){
            debug_msg = "Argument trail structure is wrong";
            return false;
        }
            
        int idx = 0;
        for(int i = 0; i < count; i+=2){
            arg_buffer[idx].name = args[i];
            arg_buffer[idx].name.replace("-", "");
            arg_buffer[idx].value = args[i+1];
            idx++;
        }
        return true;
    }
    
    String* _split(String raw) {
        if (raw.length() == 0)
            return new String[0];
        if (raw[raw.length() - 1] != ' ')
            raw += " ";
        int count = _count_spaces(raw);
        int trail = 0;
        String buffer = "";
        String *ret = new String[count];
        for (int a = 0; a < raw.length(); a++) {
            if (raw[a] == '"') {
                a++;
                while (raw[a] != '"') {
                    buffer += raw[a++];
                }
            }else if (raw[a] != ' ') {
                buffer += raw[a];
            }
            if (raw[a] == ' ') {
                ret[trail++] = buffer;
                buffer = "";
            }
        }
        return ret;
    }

    int _count_spaces(String raw) {
        int count = 0;
        for (int a = 0; a < raw.length(); a++) {
            if (raw[a] == '"') {
                a++;
                while (raw[a] != '"') {
                    a++;
                }
                a++;
            }
            if (raw[a] == ' ') {
                count++;
            }
        }
        return count;
    }

    bool _check_variable_to_value_ratio(String *args, int size){
        int count_var = 0;
        int count_val = 0;
        for (int a = 0; a < size; a++){
            if(args[a].startsWith("-"))
                count_var++;
            else
                count_val++;
        }
        return count_var == count_val;
    }

    bool _check_argument_trail_structure(String* args, int size) {
        if (args[0][0] != '-') 
            return false;
        if (args[size - 1][0] == '-')
            return false;
        if (size == 2)
            return true;
        for (int a = 0; a < size; a+=2) {
            if (args[a][0] != '-')
                return false;
        }
        return true;
    }

};

//#define serial_int serial_interface::get_instance()