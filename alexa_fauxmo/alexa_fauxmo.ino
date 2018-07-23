#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "fauxmoESP.h"

/* Network credentials */
#define WIFI_SSID "Skylake"
#define WIFI_PASS "precarious"

#define DEVICE_ONE "TV"
#define DEVICE_TWO "Speaker"
#define DEVICE_ALL "All Devices"

#define SERIAL_BAUDRATE 115200

WiFiServer server(80);

/* Belkin WeMo emulation */
fauxmoESP fauxmo;

/* Set Relay Pins */
#define RELAY_1 D5
#define RELAY_2 D6
#define LED D4

int switchedOnDeviceCount = 0;

void setup() 
{
   // set pin modes
   pinSetup();

   // start serial
   Serial.begin(SERIAL_BAUDRATE);
   
   // setup and wifi connection
   wifiSetup();
    
   // Device Names for Simulated Wemo switches
   fauxmo.addDevice(DEVICE_ONE);
   fauxmo.addDevice(DEVICE_TWO);
   fauxmo.addDevice(DEVICE_ALL);
   fauxmo.onMessage(callback); 
   
   server.begin();
}

void loop() 
{
  fauxmo.handle();
  blinkLED();
  webServer();
}

void webServer() {
  WiFiClient client = server.available();
  if(!client) {
    return;
  }

  while(!client.available()) {
    Serial.println("Not Returning");
    delay(1);
  }

  String request = client.readStringUntil('\r');
  Serial.println("Request - ");
  Serial.println(request);

  // client.flush();

  if(request.indexOf("/tv/1") != -1) {
    switchOnRelay(RELAY_1);
  }
  else if(request.indexOf("/tv/0") != -1) {
    switchOffRelay(RELAY_1);
  }
  else if(request.indexOf("/speaker/1") != -1) {
    switchOnRelay(RELAY_2);
  }
  else if(request.indexOf("/speaker/0") != -1) {
    switchOffRelay(RELAY_2);
  }
  else if(request.indexOf("/all/1") != -1) {
    switchOnRelays();
  }
  else if(request.indexOf("/all/0") != -1) {
    switchOffRelays();
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  
  client.println("<br><br>");
  client.println("<a href=\"/tv/1\"\"><button>Turn On TV</button></a>");
  client.println("<a href=\"/tv/0\"\"><button>Turn Off TV</button></a><br />");
  client.println("<br><br>");
  client.println("<a href=\"/speaker/1\"\"><button>Turn On Speaker</button></a>");
  client.println("<a href=\"/speaker/0\"\"><button>Turn Off Speaker</button></a><br />");
  client.println("<br><br>");
  client.println("<a href=\"/all/1\"\"><button>Turn On All Devices</button></a>");
  client.println("<a href=\"/all/0\"\"><button>Turn Off All Devices</button></a><br />");
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}

void blinkLED() {
  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(3000);                       // wait for a second

  if(switchedOnDeviceCount < 1)
  {
    digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
    delay(200);
  }
}

void pinSetup()
{
   pinMode(LED, OUTPUT);
 
   //Set relay pins to outputs
   pinMode(RELAY_1, OUTPUT);
   pinMode(RELAY_2, OUTPUT);

   //Set each relay pin to HIGH ====== NOTE THAT THE RELAYS USE INVERSE LOGIC =====
   digitalWrite(RELAY_1, HIGH);   
   digitalWrite(RELAY_2, HIGH);
}

/* ---------------------------------------------------------------------------
 Device Callback
 ----------------------------------------------------------------------------*/
void callback(uint8_t device_id, const char * device_name, bool state) 
{
  logger(device_name, state);
  
  //Switching action on detection of device name
  if ( (strcmp(device_name, DEVICE_ONE) == 0) ) 
  {
    if (!state) 
    {
      switchOffRelay(RELAY_1);
    } 
    else 
    {
      switchOnRelay(RELAY_1);
    }
  }

  if ( (strcmp(device_name, DEVICE_TWO) == 0) ) 
  {
    if (!state) 
    {
      switchOffRelay(RELAY_2);
    } 
    else 
    {
      switchOnRelay(RELAY_2);
    }
  }
 
  if ( (strcmp(device_name, DEVICE_ALL) == 0) ) 
  {
    if (!state) 
    {
      switchOffRelays();
    } 
    else 
    {
      switchOnRelays();
    }
  }

  Serial.printf("Number of device on: %d", switchedOnDeviceCount);
}
    
/* -----------------------------------------------------------------------------
 Wifi Setup
 -----------------------------------------------------------------------------*/
void wifiSetup() 
{
   // Set WIFI module to STA mode
   // WiFi.mode(WIFI_STA);

   // Connect
   Serial.println ();
   Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
   Serial.println();
   WiFi.begin(WIFI_SSID, WIFI_PASS);

   // Wait
   while (WiFi.status() != WL_CONNECTED) 
   {
      Serial.print(".");
      delay(100);
   }
   Serial.println("CONNECTED!");
   Serial.println();

   // Connected!
   Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
   Serial.println();
}

void switchOnRelay(int relay)
{
  digitalWrite(relay, LOW);
  switchedOnDeviceCount = switchedOnDeviceCount + 1;
}

void switchOffRelay(int relay)
{
  digitalWrite(relay, HIGH);
  switchedOnDeviceCount = switchedOnDeviceCount - 1;
}

void switchOnRelays()
{
  switchOnRelay(RELAY_1);
  switchOnRelay(RELAY_2);
}

void switchOffRelays()
{
  switchOffRelay(RELAY_1);
  switchOffRelay(RELAY_2);
}

void logger(const char * device_name, bool state)
{
  Serial.print("Device "); Serial.print(device_name); 
  Serial.print(" state: ");
  if (state) 
  {
    Serial.println("ON");
  } 
  else 
  {
    Serial.println("OFF");
  }
}
