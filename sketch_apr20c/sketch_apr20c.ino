#include <WiFiClientSecure.h>

#define SERIAL_BAUDRATE 115200

WiFiClientSecure client;

void setup() {
  SkylakeWifiSetup(SERIAL_BAUDRATE);

  // FirebaseGenericApiConnection();
  makeConnection();
  postData();
}

void loop() {
  
}

