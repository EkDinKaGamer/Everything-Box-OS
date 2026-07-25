#ifndef EBOS_JOYSTICK_H
#define EBOS_JOYSTICK_H
#include <Arduino.h>
#include "../core/Navigation.h"

#define JOY_X A0
#define JOY_Y A1
#define JOY_SW 2 // BTN_A
#define BTN_B 12
#define BTN_C A2
#define BTN_D A3

class Joystick {
  int lastX, lastY;
  unsigned long lastDebounce[4];
  bool lastStateA, lastStateB, lastStateC, lastStateD;
  NavEvent pending = NAV_NONE;
public:
  void begin() {
    pinMode(JOY_SW, INPUT_PULLUP);
    pinMode(BTN_B, INPUT_PULLUP);
    pinMode(BTN_C, INPUT_PULLUP);
    pinMode(BTN_D, INPUT_PULLUP);
    lastX = analogRead(JOY_X);
    lastY = analogRead(JOY_Y);
    lastStateA = digitalRead(JOY_SW)==LOW;
    lastStateB = digitalRead(BTN_B)==LOW;
    lastStateC = digitalRead(BTN_C)==LOW;
    lastStateD = digitalRead(BTN_D)==LOW;
    for (int i=0;i<4;i++) lastDebounce[i]=0;
  }
  void update() {
    int x = analogRead(JOY_X);
    int y = analogRead(JOY_Y);
    // directional thresholds with hysteresis
    if (x < 200 && lastX >= 200) pending = NAV_LEFT;
    else if (x > 800 && lastX <= 800) pending = NAV_RIGHT;
    else if (y < 200 && lastY >= 200) pending = NAV_UP;
    else if (y > 800 && lastY <= 800) pending = NAV_DOWN;
    lastX = x; lastY = y;

    // buttons with simple debounce
    bool a = digitalRead(JOY_SW)==LOW;
    if (a != lastStateA && millis() - lastDebounce[0] > 40) {
      lastDebounce[0] = millis(); lastStateA = a;
      if (a) pending = NAV_BTN_A; // press
    }
    bool b = digitalRead(BTN_B)==LOW;
    if (b != lastStateB && millis() - lastDebounce[1] > 40) {
      lastDebounce[1] = millis(); lastStateB = b;
      if (b) pending = NAV_BTN_B;
    }
    bool c = digitalRead(BTN_C)==LOW;
    if (c != lastStateC && millis() - lastDebounce[2] > 40) {
      lastDebounce[2] = millis(); lastStateC = c;
      if (c) pending = NAV_BTN_C;
    }
    bool d = digitalRead(BTN_D)==LOW;
    if (d != lastStateD && millis() - lastDebounce[3] > 40) {
      lastDebounce[3] = millis(); lastStateD = d;
      if (d) pending = NAV_BTN_D;
    }
  }
  NavEvent consumeEvent() {
    NavEvent r = pending; pending = NAV_NONE; return r;
  }
};

#endif
