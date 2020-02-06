/*
 Basic ESP8266 MQTT example

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>  

// Update these with values suitable for your network.

const char* ssid = "Skylake";
const char* password = "precarious";
const char* mqtt_server = "farmer.cloudmqtt.com";

WiFiClient espClient;
PubSubClient client(espClient);

#define LED D4

#define RELAY_1 D5
#define RELAY_2 D6
#define RELAY_3 D7
#define RELAY_4 D8

#define DEVICE_ONE_QUEUE "tv-queue"
#define DEVICE_TWO_QUEUE "speaker-queue"

#define DEVICE_ONE_EVENT "tv-event"
#define DEVICE_TWO_EVENT "speaker-event"


void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting...");

   WiFiManager wifiManager;

   if (!wifiManager.autoConnect("Mqtt-Switch")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if(strcmp(topic, DEVICE_ONE_QUEUE) == 0) {
    Serial.println("Resolved to TV topic");
    if ((char)payload[0] == '1') {
       switchOnTv();
    } else {
      switchOffTv();
    }
  } else if(strcmp(topic, DEVICE_TWO_QUEUE) == 0) {
    Serial.println("Resolved to Speaker topic");
    if ((char)payload[0] == '1') {
      switchOnSpeaker();
    } else {
      switchOffSpeaker();
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "switch-hub";
    // Attempt to connect
    if (client.connect(clientId.c_str(), "ryzencvx", "YotDoRJP_I8w")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe(DEVICE_ONE_QUEUE);
      client.subscribe(DEVICE_TWO_QUEUE);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);

  digitalWrite(RELAY_1, HIGH);   
  digitalWrite(RELAY_2, HIGH);

   // Always on pins
   digitalWrite(RELAY_3, LOW);   
   digitalWrite(RELAY_4, LOW);
   
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 11772);
  client.setCallback(callback);
}

void loop() {
  digitalWrite(LED, HIGH); 
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void switchOnTv()
{
  switchOnRelay(RELAY_1);
  client.publish(DEVICE_ONE_EVENT, "1");
  Serial.println("TV switched on.");
}

void switchOffTv()
{
  switchOffRelay(RELAY_1);
  client.publish(DEVICE_ONE_EVENT, "0");
  Serial.println("TV switched off.");
}

void switchOnSpeaker()
{
  switchOnRelay(RELAY_2);
  client.publish(DEVICE_TWO_EVENT, "1");
  Serial.println("Speaker switched on.");
}

void switchOffSpeaker()
{
  switchOffRelay(RELAY_2);
  client.publish(DEVICE_TWO_EVENT, "0");
  Serial.println("Speaker switched off.");
}

void switchOnRelay(int relay)
{
  digitalWrite(relay, LOW);

  digitalWrite(LED, LOW);
  delay(200);
}

void switchOffRelay(int relay)
{
  digitalWrite(relay, HIGH);
  
  digitalWrite(LED, LOW);
  delay(200);
}
