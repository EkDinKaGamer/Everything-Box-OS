#ifndef EBOS_COMM_ESP_H
#define EBOS_COMM_ESP_H
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <time.h>

// Simple serial command handler for ESP side. Commands are newline-terminated.
// Supported commands:
// WIFI:SCAN
// WIFI:CONNECT:ssid:pass
// TIME:SYNC
// Response examples:
// WIFI:SCAN:SSID1;SSID2\n
class CommESP {
  static Stream* ser;
public:
  static void begin(Stream &s) { ser = &s; }
  static void poll() {
    if (!ser) return;
    if (!ser->available()) return;
    String line = ser->readStringUntil('\n');
    if (line.length()==0) return;
    // parse
    if (line.startsWith("WIFI:SCAN")) {
      int n = WiFi.scanNetworks();
      String out = "WIFI:SCAN:";
      for (int i=0;i<n && i<10;i++) {
        out += WiFi.SSID(i);
        if (i < n-1) out += ";";
      }
      ser->println(out);
    } else if (line.startsWith("WIFI:CONNECT:")) {
      // WIFI:CONNECT:ssid:pass
      int p1 = line.indexOf(':',13);
      if (p1>12) {
        String ssid = line.substring(13, p1);
        String pass = line.substring(p1+1);
        ssid.trim(); pass.trim();
        ser->println("WIFI:CONNECTING");
        WiFi.begin(ssid.c_str(), pass.c_str());
        int t=0;
        while (WiFi.status() != WL_CONNECTED && t < 100) { delay(100); t++; }
        if (WiFi.status() == WL_CONNECTED) {
          ser->print("WIFI:CONNECTED:"); ser->println(WiFi.localIP());
        } else {
          ser->println("WIFI:FAILED");
        }
      }
    } else if (line.startsWith("TIME:SYNC")) {
      // get NTP time using config
      configTime(0,0,"pool.ntp.org","time.nist.gov");
      struct tm timeinfo;
      if (getLocalTime(&timeinfo, 3000)) {
        char buf[32];
        sprintf(buf,"TIME:%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
        ser->println(buf);
      } else {
        ser->println("TIME:ERR");
      }
    } else {
      ser->println("UNK");
    }
  }
};

Stream* CommESP::ser = nullptr;

#endif
