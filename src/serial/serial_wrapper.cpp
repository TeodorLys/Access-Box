#include "serial_wrapper.h"

WiFiUDP serial_wrapper::udp;
String serial_wrapper::broadcast_ip = "0.0.0.0";
int serial_wrapper::port = -1;