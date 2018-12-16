#include <ESP8266WiFi.h>

class WifiSetup {
  private:
    String ssid;
    String password;

  public:
    WifiSetup() {
      this->ssid = "Skylake";
      this->password = "precarious";
    }

    void connect() {
      // Connect
     Serial.println ();
     Serial.printf("[WIFI] Connecting to %s ", this->ssid.c_str());
     Serial.println();
     
     WiFi.begin(this->ssid.c_str(), this->password.c_str());
  
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
};


