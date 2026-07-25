#ifndef EBOS_COMM_NANO_H
#define EBOS_COMM_NANO_H
#include <Arduino.h>

class Communication {
  static Stream* ser;
  static char respBuf[128];
  static uint8_t respLen;
public:
  static void begin(Stream &s) { ser = &s; respLen = 0; }
  static void begin(Stream *s) { ser = s; respLen = 0; }
  static void request(const char* cmd) {
    if (!ser) return;
    ser->println(cmd);
  }
  // call periodically to read responses into internal buffer
  static void poll() {
    if (!ser) return;
    while (ser->available() && respLen < (sizeof(respBuf)-1)) {
      int c = ser->read();
      if (c == '\n') { respBuf[respLen] = '\0'; respLen++; break; }
      respBuf[respLen++] = (char)c;
    }
    if (respLen >= (sizeof(respBuf)-1)) respBuf[sizeof(respBuf)-1]=0;
  }
  static bool availableResponse() { poll(); return respLen>0; }
  static int readResponse(char* out, int maxlen) {
    if (respLen==0) return 0;
    int n = strlcpy(out, respBuf, maxlen);
    respLen = 0; respBuf[0]=0;
    return n;
  }
};

Stream* Communication::ser = nullptr;
char Communication::respBuf[128];
uint8_t Communication::respLen = 0;

#endif
