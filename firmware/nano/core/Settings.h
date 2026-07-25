#ifndef EBOS_SETTINGS_H
#define EBOS_SETTINGS_H
#include <Arduino.h>
#include "Storage.h"

struct Settings {
  uint8_t brightness; // 0-15
  bool soundOn;
  bool bootAnimation;
  uint8_t rgbR;
  uint8_t rgbG;
  uint8_t rgbB;

  void begin() {
    Storage s; s.begin();
    s.loadSettings(*this);
  }
  void save() {
    Storage s; s.saveSettings(*this);
  }
};

#endif
