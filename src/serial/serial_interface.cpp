#include "serial_interface.h"

mode_handler *serial_interface::_handler;
bool serial_interface::prog_mode_state = false;
dict serial_interface::arg_buffer[10];