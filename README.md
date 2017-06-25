# LinkNodeR4 firmware

## Documentation
The following MQTT topics are subscribed to by the client;
* HeatingControl/heartbeat - A heartbeat to tell the relay the controller is alive.
* HeatingControl/Cmd/Relay1 - 1 switches relay on, 0 switches it off
* HeatingControl/Cmd/Relay2
* HeatingControl/Cmd/Relay3
* HeatingControl/Cmd/Relay4

The following topics are published by the LinkNodeR4 - this is the state the system knows the relays are in:
* HeatingControl/Status/Status
* HeatingControl/Status/Relay1
* HeatingControl/Status/Relay2
* HeatingControl/Status/Relay3
* HeatingControl/Status/Relay4

Put your own values for Wifi and IP address in.  An IP address of 0.0.0.0 will force DHCP.  Save the file as 'LocalSettings.hpp'. This will be ignored by git.

