LinknodeR4 firmware

Documentation
The following MQTT topics are subscribed to;
**HeatingControl/heartbeat - A heartbeat to tell the relay the controller is alive.
**HeatingControl/Cmd/Relay1 - 1 switches relay on, 0 switches it off
**HeatingControl/Cmd/Relay2
**HeatingControl/Cmd/Relay3
**HeatingControl/Cmd/Relay4

The following topics are published
HeatingControl/Status/Status
HeatingControl/Status/Relay1
HeatingControl/Status/Relay2
HeatingControl/Status/Relay3
HeatingControl/Status/Relay4
