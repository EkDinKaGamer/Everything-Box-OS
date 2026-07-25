#pragma once
#include <Arduino.h>
#include "../modules/Display.h"
#include "../modules/Input.h"

class EBOS_Diagnostics {
public:
  void bootCheck() {
    Serial.println("EBOS Diagnostics Start");
    Serial.print("Joystick X: ");
    Serial.println(Input.x());
    Serial.print("Joystick Y: ");
    Serial.println(Input.y());
  }
};

EBOS_Diagnostics Diagnostics;
