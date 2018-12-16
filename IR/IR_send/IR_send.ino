#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRsend.h>

const uint16_t kIrLed = D2;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.

// Example of data captured by IRrecvDumpV2.ino
uint16_t rawData[67] = {9000, 4500, 650, 550, 650, 1650, 600, 550, 650, 550,
                        600, 1650, 650, 550, 600, 1650, 650, 1650, 650, 1650,
                        600, 550, 650, 1650, 650, 1650, 650, 550, 600, 1650,
                        650, 1650, 650, 550, 650, 550, 650, 1650, 650, 550,
                        650, 550, 650, 550, 600, 550, 650, 550, 650, 550,
                        650, 1650, 600, 550, 650, 1650, 650, 1650, 650, 1650,
                        650, 1650, 650, 1650, 650, 1650, 600};

void setup() {
  irsend.begin();
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
}

void loop() {
#if SEND_NEC
  Serial.println("NEC");
  irsend.sendNEC(0x00FFE01FUL, 32);
#endif  // SEND_NEC
  delay(2000);
#if SEND_SONY
  Serial.println("Sony");
  irsend.sendSony(0xa90, 12, 2);
#endif  // SEND_SONY
  delay(2000);
#if SEND_RAW
  Serial.println("a rawData capture from IRrecvDumpV2");
  irsend.sendRaw(rawData, 67, 38);  // Send a raw data capture at 38kHz.
#endif  // SEND_RAW
  delay(2000);
}
