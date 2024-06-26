# Access-Box
A battery driven access-point for when troubleshooting networks. With integrated rs232 reader and ESP32-C3 chip
ESP32 chip has a couple of modes;
Default: Displays ESP ip address, gateway address, wan status and udp port for reading and writing to the RS232 port
Pingtest: Displays the pinging ip address, average milliseconds and wan status.

To initialize the UDP connection we need to tell the ESP32 chip what IP we are sending from.
to do this we send the Init command and the password.

```
init <password> <device name>
```

## Changing states and changing settings:
To enter programming mode on the ESP chip, send the command below to via UDP 
```
enter prog mode
#RET: Programming mode is activated!
```
To enter modes:
```
mode <mode_name> <arguments[-name value] ex. -ip 192.168.50.1>
#RET: OK / FAILED: <MESSAGE>
```
To exit programming mode:
```
exit prog mode
#RET: Programming mode is deactivated!
```

## Modes
### Default:
```
mode default <arguments>
```
#### Available Arguments:
NONE
### PingTest:
```
mode pingtest <arguments>
```
#### Available Arguments:
IP: what ip the ping function will use
```
mode pingtest -ip 192.168.1.1
```
### New WiFi:
```
mode newwifi <arguments>
```
#### Available Arguments:
SSID: what ssid it should connect to
```
mode newwifi -ssid Teodors
```
PSK: Password for the ssid
```
mode newwifi -psk abc123
```

