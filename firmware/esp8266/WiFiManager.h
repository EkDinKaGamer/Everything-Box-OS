#ifndef EBOS_WIFI_ESP_H
#define EBOS_WIFI_ESP_H
#include <ESP8266WiFi.h>
#include <Arduino.h>

class WiFiManagerESP {
public:
  static void begin() {
    WiFi.mode(WIFI_STA);
    WiFi.setAutoConnect(true);
  }
  static bool connected() { return WiFi.status() == WL_CONNECTED; }
  static IPAddress localIP() { return WiFi.localIP(); }
};

#endif
