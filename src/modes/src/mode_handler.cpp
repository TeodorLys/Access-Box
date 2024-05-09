#include "../header/mode_handler.h"


void mode_handler::process(){
    _active_mode->process();
}