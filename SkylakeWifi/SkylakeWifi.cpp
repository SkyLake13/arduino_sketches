#include "SkylakeWifi.h"
#include <ESP8266WiFi.h>

SkylakeWifi::SkylakeWifi()
{
  _ssid = "Skylake";
  _password = "precarious";
}


SkylakeWifi::~SkylakeWifi()
{
  WiFi.disconnect();
  _ssid = "";
  _password = "";
}

void SkylakeWifi::connect()
{
  Serial.print("connecting to ");
  Serial.println(_ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(_ssid, _password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void SkylakeWifi::disconnect()
{
  WiFi.disconnect();
}
