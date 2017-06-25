// File for local settings.  Use .gitignore so this is not uploaded to GitHub

// Edit this file and save as LocalSettings.hpp

// DO NOT PUT YOUR DETAILS IN AND PUSH CHANGES

#include <ESP8266WiFi.h>

//enironment setup
const char* ssid = "X";
const char* password = "XXX";
const char* mqtt_server = "192.168.0.0";

// Edit these 3 lines for a fixed IP-address
// For DCHP, just comment them out (or delete them)
IPAddress ip(192, 168, 0, 0);
IPAddress gateway(192, 168, 0, 0);
IPAddress subnet(255, 255, 255, 0);
