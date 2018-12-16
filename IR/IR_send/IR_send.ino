
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <ESP8266WebServer.h>
#include <IRsend.h>
#include "Speaker.h";
#include "Tv.h";
#include "WifiSetup.h";

const uint16_t transmitterPin = D2;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).

WifiSetup wifiSetup;

WiFiServer server(80);

IRsend irsend(transmitterPin); // Set the GPIO to be used to sending the message.

Speaker speaker(irsend);

Tv tv(irsend);

void setup() {
  wifiSetup.connect();
  
  server.begin();
  
  irsend.begin();
    
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
}

void loop() {
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

  if(request.indexOf("/tv/power") != -1) {
    tv.powerToggle();
  }
  else if(request.indexOf("/tv/source") != -1) {
    tv.changeSource();
  }

  
  else if(request.indexOf("/speaker/power") != -1) {
    speaker.powerToggle();
  }
  else if(request.indexOf("/speaker/mode") != -1) {
    speaker.changeMode();
  }
  else if(request.indexOf("/speaker/up") != -1) {
    speaker.volumeUp();
  }
  else if(request.indexOf("/speaker/down") != -1) {
    speaker.volumeDown();
  }

  String json = "";
  json.concat("{\"running\":\"true\"}");

  Serial.println(json);

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println(); //  do not forget this one
  client.println(json);
  client.println(); //  do not forget this one
  
  client.flush();
  
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}

