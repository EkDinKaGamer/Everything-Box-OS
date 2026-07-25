#ifndef EBOS_STORAGE_H
#define EBOS_STORAGE_H
#include <Arduino.h>
#include <EEPROM.h>
#include "Settings.h"

// EEPROM layout
// 0: signature
// 1: brightness
// 2: soundOn
// 3: bootAnimation
// 4: rgbR
// 5: rgbG
// 6: rgbB
// 10.. pixel drawings (8 bytes each)

class Storage {
public:
  void begin() {}
  void saveSettings(const Settings &s) {
    EEPROM.update(0, 0xEA);
    EEPROM.update(1, s.brightness);
    EEPROM.update(2, s.soundOn ? 1 : 0);
    EEPROM.update(3, s.bootAnimation ? 1 : 0);
    EEPROM.update(4, s.rgbR);
    EEPROM.update(5, s.rgbG);
    EEPROM.update(6, s.rgbB);
  }
  void loadSettings(Settings &s) {
    if (EEPROM.read(0) != 0xEA) {
      // defaults
      s.brightness = 8;
      s.soundOn = true;
      s.bootAnimation = true;
      s.rgbR = 255; s.rgbG = 0; s.rgbB = 0;
      saveSettings(s);
      return;
    }
    s.brightness = EEPROM.read(1);
    s.soundOn = EEPROM.read(2) != 0;
    s.bootAnimation = EEPROM.read(3) != 0;
    s.rgbR = EEPROM.read(4);
    s.rgbG = EEPROM.read(5);
    s.rgbB = EEPROM.read(6);
  }
  void saveDrawing(uint8_t slot, const uint8_t *frame) {
    uint16_t base = 20 + slot*8;
    for (uint8_t i=0;i<8;i++) EEPROM.update(base+i, frame[i]);
  }
  void loadDrawing(uint8_t slot, uint8_t *frame) {
    uint16_t base = 20 + slot*8;
    for (uint8_t i=0;i<8;i++) frame[i] = EEPROM.read(base+i);
  }
  void pollSave() {}
};

#endif
