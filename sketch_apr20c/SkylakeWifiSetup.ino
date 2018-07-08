#include <ESP8266WiFi.h>

const char* ssid = "Skylake";
const char* password = "precarious";

void SkylakeWifiSetup(int serialPortBaudRate) {
  Serial.begin(serialPortBaudRate);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
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

