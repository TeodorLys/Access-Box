# Access-Box
A battery driven access-point for when troubleshooting networking. With integrated rs232 reader and ESP32-C3 chip
ESP32 chip has a couple of modes;
Default: Displays ESP ip address, gateway address, wan status and udp port for reading and writing to the RS232 port
Pingtest: Displays the pinging ip address, average milliseconds and wan status.

## Changing states and changing settings:
To enter programming mode on the ESP chip, send the command below to via UDP 
```
enter prog mode
```
It will return
```
Programming mode is activated!
```
To exit programming mode:
```
exit prog mode
```
To enter modes:
```
mode <mode_name> <arguments[-name value] ex. -ip 192.168.50.1>
```
