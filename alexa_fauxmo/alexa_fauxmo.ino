#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <IRsend.h>

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "fauxmoESP.h"

/* Network credentials */
#define WIFI_SSID "Skylake"
#define WIFI_PASS "precarious"

#define DEVICE_ONE "TV"
#define DEVICE_TWO "Speaker"

#define SERIAL_BAUDRATE 115200

WiFiServer server(80);

/* Belkin WeMo emulation */
fauxmoESP fauxmo;

/* Set Relay Pins */
#define IR_TRANSMETER D2
#define IR_RECEIVER D3

#define LED D4

#define RELAY_1 D5
#define RELAY_2 D6
#define RELAY_3 D7
#define RELAY_4 D8

IRrecv irrecv(IR_RECEIVER);
decode_results results;

IRsend irsend(IR_TRANSMETER);  // Set the GPIO to be used to sending the message.

bool statusTv = false;
bool statusSpeaker = false;

uint16_t rawData[1] = {21516};

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

   fauxmo.onMessage(callback); 
   
   server.begin();
   irrecv.enableIRIn(); // Start the receiver
   irsend.begin();
}

void loop() 
{
  fauxmo.handle();
  blinkLED();
  webServer();

  irReceiver();
}

void irReceiver()
{
  if (irrecv.decode(&results))
  {
     if (results.decode_type == NEC) {
        Serial.print("NEC: ");
      } else if (results.decode_type == SONY) {
        Serial.print("SONY: ");
      } else if (results.decode_type == RC5) {
        Serial.print("RC5: ");
      } else if (results.decode_type == RC6) {
        Serial.print("RC6: ");
      } else if (results.decode_type == UNKNOWN) {
        Serial.print("UNKNOWN: ");
      }
      serialPrintUint64(results.value, HEX);
      serialPrintUint64(results.bits);
      Serial.println();
      irrecv.resume(); // Receive the next value
  }
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
    switchOnTv();
  }
  else if(request.indexOf("/tv/0") != -1) {
    switchOffTv();
  }
  else if(request.indexOf("/speaker/1") != -1) {
    switchOnSpeaker();
  }
  else if(request.indexOf("/speaker/0") != -1) {
    switchOffSpeaker();
  }
  else if(request.indexOf("/all/1") != -1) {
    switchOnAll();
  }
  else if(request.indexOf("/all/0") != -1) {
    switchOffAll();
  }

  String json = "[";
  json.concat("{\"name\":\"TV\", \"url\":\"tv\", \"state\":");
  json.concat(statusTv);
  json.concat("},");
  json.concat("{\"name\":\"Speaker\", \"url\":\"speaker\", \"state\":");
  json.concat(statusSpeaker);
  json.concat("}");
  json.concat("]");

  Serial.println(json);

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println(); //  do not forget this one
  client.println(json);
  client.println(); //  do not forget this one
    
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}

void blinkLED() {
  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(3000);                       // wait for a second

  if(!(statusSpeaker || statusTv))
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
   pinMode(RELAY_3, OUTPUT);
   pinMode(RELAY_4, OUTPUT);

   //Set each relay pin to HIGH ====== NOTE THAT THE RELAYS USE INVERSE LOGIC =====
   digitalWrite(RELAY_1, HIGH);   
   digitalWrite(RELAY_2, HIGH);

   // Always on pins
   digitalWrite(RELAY_3, LOW);   
   digitalWrite(RELAY_4, LOW);
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
      switchOffTv();
    } 
    else 
    {
      switchOnTv();
    }
  }

  if ( (strcmp(device_name, DEVICE_TWO) == 0) ) 
  {
    if (!state) 
    {
      switchOffSpeaker();
    } 
    else 
    {
      switchOnSpeaker();
    }
  }
}

void switchOnTv()
{
  switchOnRelay(RELAY_1);
  statusTv = true;
}

void switchOffTv()
{
  switchOffRelay(RELAY_1);
  statusTv = false;
}

void switchOnSpeaker()
{
  switchOnRelay(RELAY_2);
  statusSpeaker = true;
  delay(500);
  irsend.sendSony(0x540C, 15, 2);
}

void switchOffSpeaker()
{
  switchOffRelay(RELAY_2);
  statusSpeaker = false;
}

void switchOnAll()
{
  switchOnTv();
  switchOnSpeaker();
}

void switchOffAll()
{
  switchOffTv();
  switchOffSpeaker();
}

void switchOnRelay(int relay)
{
  digitalWrite(relay, LOW);
}

void switchOffRelay(int relay)
{
  digitalWrite(relay, HIGH);
}
    
/* -----------------------------------------------------------------------------
 Wifi Setup
 -----------------------------------------------------------------------------*/
void wifiSetup() 
{
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
