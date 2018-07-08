#include <WiFiClientSecure.h>

const char* host = "us-central1-boreal-physics-153205.cloudfunctions.net";
const int httpsPort = 443;

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
const char* fingerprint = "7f 43 c3 6e 65 8d 15 94 ba b9 2d b7 52 d4 a3 70 06 91 69 01";

String url = "/genericstore/employee/";

void FirebaseGenericApiConnection() {
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");
}

void makeConnection() {
  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  if (client.verify(fingerprint, host)) {
    Serial.println("certificate matches");
  } else {
    Serial.println("certificate doesn't match");
  }
}

void postData() {
  String data = "title=foo&body=bar&userId=1";
  String request = "POST " + url + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "User-Agent: ESP8266\r\n" +
                   "Content-Length: " + data.length() + "\r\n" +
                   "Content-Type: application/x-www-form-urlencoded\r\n\r\n" +
                   data + "\r\n";
               
  client.println(request);
  Serial.println(request);

  while (client.connected()) {
    Serial.println("request sent");
    if (client.available())
    {
      char str = client.read();
      Serial.println("reply was:");
      Serial.println("==========");
      Serial.println(str);
      Serial.println("==========");
    }      
  }
  
  Serial.println("closing connection");
}

