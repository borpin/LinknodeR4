#include "LocalSettings.hpp"
#include <PubSubClient.h>
#include <Ticker.h>

//MQTT Topics
const char* MQTTdeviceIDStr = "HeatingControl";
const char* HeartBeat = "HeatingControl/HeartBeat";
const char* Relay1Cmd = "HeatingControl/Cmd/Relay1";
const char* Relay2Cmd = "HeatingControl/Cmd/Relay2";
const char* Relay3Cmd = "HeatingControl/Cmd/Relay3";
const char* Relay4Cmd = "HeatingControl/Cmd/Relay4";

const char* LinkHeartBeat = "HeatingControl/Status/Status";
const char* Relay1Status = "HeatingControl/Status/Relay1";
const char* Relay2Status = "HeatingControl/Status/Relay2";
const char* Relay3Status = "HeatingControl/Status/Relay3";
const char* Relay4Status = "HeatingControl/Status/Relay4";

const int ControlTimeout = 60; //Watchdog timer
// stored status values
bool Relay1Statusb;
bool Relay2Statusb;
bool Relay3Statusb;
bool Relay4Statusb;

bool ExtContOK; // Used to check and see if a heartbeat has been received from the controller

void reconnect();

WiFiClient espClient;
PubSubClient client(espClient);
Ticker Watchdog;

void setup_wifi(){

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  //Comment these 3 lines out for DHCP IP
  WiFi.mode(WIFI_STA);
  WiFi.hostname(MQTTdeviceIDStr);     // DHCP Hostname (useful for finding device for static lease)
  WiFi.config(ip, gateway, subnet);  // (DNS not required)

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void AreWeStillAlive (){
  // checks to see if the controlling system is still alive.
  // If no message is sent the pins are all set LOW/off
  // As the Linknode can be wired as NO or NC off can be assumed
  // as a safe state.
  Serial.println("Checking controller");

  // The flag will be set by the External Controller regularly
  // set to false so controller sets to true.
  if (ExtContOK) {
    ExtContOK = !ExtContOK;
    Serial.println(ExtContOK);
  }
  else {
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
    digitalWrite(14, LOW);
    digitalWrite(16, LOW);
    Relay1Statusb = false;
    Relay2Statusb = false;
    Relay3Statusb = false;
    Relay4Statusb = false;
    Serial.println("No External Controller");
  }
  // Always publish our heartbeat
//  client.publish(LinkHeartBeat, "1");
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  int payloadi = (int)payload[0] - 48; //need to subtract 48 as chr 0 = int 48!

  // This looks to see which topic sent the message
  // Set the appropriate pin.
  // Set the state machine state to return.
  if ((strcmp(topic, Relay1Cmd)) == 0) {
    digitalWrite(12, payloadi);
    Relay1Statusb = payloadi;
  } else if ((strcmp(topic, Relay2Cmd)) == 0) {
    digitalWrite(13, payloadi);
    Relay2Statusb = payloadi;
  } else if ((strcmp(topic, Relay3Cmd)) == 0) {
    digitalWrite(14, payloadi);
    Relay3Statusb = payloadi;
  } else if ((strcmp(topic, Relay4Cmd)) == 0) {
    digitalWrite(16, payloadi);
    Relay4Statusb = payloadi;
  } else if ((strcmp(topic, HeartBeat)) == 0) {
//    client.publish(LinkHeartBeat, "1");
  } else {
    Serial.println("Bad Message");
  }
  ExtContOK = true; // Any message received
  // Output the status of every relay every time.
  if (!client.connected()) {
    reconnect();
  }
  client.publish(LinkHeartBeat, "1");
  client.publish(Relay1Status,Relay1Statusb ? "1" : "0");
  client.publish(Relay2Status,Relay2Statusb ? "1" : "0");
  client.publish(Relay3Status,Relay3Statusb ? "1" : "0");
  client.publish(Relay4Status,Relay4Statusb ? "1" : "0");
}

void reconnect() {
  // Loop until we're reconnected
  bool success;
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(MQTTdeviceIDStr)) {
      Serial.println(" MQTT connected");
      // ... and resubscribe
      Serial.print(" Sub HeartBeat ");
      Serial.println(client.subscribe(HeartBeat)? "True" : "False");
      Serial.print(" Sub Relay1Cmd ");
      Serial.println(client.subscribe(Relay1Cmd)? "True" : "False");
      Serial.print(" Sub Relay2Cmd ");
      Serial.println(client.subscribe(Relay2Cmd)? "True" : "False");
      Serial.print(" Sub Relay3Cmd ");
      Serial.println(client.subscribe(Relay3Cmd)? "True" : "False");
      Serial.print(" Sub Relay4Cmd ");
      Serial.println(client.subscribe(Relay4Cmd)? "True" : "False");

      // Once connected, publish an announcement...
      client.publish(LinkHeartBeat, "1");
      // Output the status of every relay.
//      Serial.println(" Pub Relay1Status" + client.publish(Relay1Status,Relay1Statusb ? "1" : "0"));
//      Serial.println(" Pub Relay2Status" + client.publish(Relay2Status,Relay2Statusb ? "1" : "0"));
//      Serial.println(" Pub Relay3Status" + client.publish(Relay3Status,Relay3Statusb ? "1" : "0"));
//      Serial.println(" Pub Relay4Status" + client.publish(Relay4Status,Relay4Statusb ? "1" : "0"));
      Serial.println("Reconnect Complete");
    }
    else {
      Serial.print("failed, rc=");
      delay(5000);
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  ExtContOK = true; // assume controller Ok to start with
  Relay1Statusb = false; //Relay Off
  Relay2Statusb = false; //Relay Off
  Relay3Statusb = false; //Relay Off
  Relay4Statusb = false; //Relay Off

  pinMode(12,OUTPUT), pinMode(13,OUTPUT),pinMode(14,OUTPUT),pinMode(16,OUTPUT);
  // ensure all relays are off to LOW - assumed safe
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  digitalWrite(14, LOW);
  digitalWrite(16, LOW);
  //open debug console
  Serial.begin(115200);
  // setup the WiFi
  setup_wifi();
  // set up the MQTT
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  // Connect to MQTT Broker
  reconnect();
  // attatch a watchdog to the check routine.
  Watchdog.attach (ControlTimeout, AreWeStillAlive);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  if (client.loop()){
    Serial.print("*");
    delay(1000);
  };
}
