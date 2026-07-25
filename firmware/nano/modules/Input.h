#pragma once
#include <Arduino.h>
#include "../Config.h"

class EBOS_Input {
public:
 void begin(){pinMode(JOYSTICK_SW,INPUT_PULLUP);}
 int x(){return analogRead(JOYSTICK_X);}
 int y(){return analogRead(JOYSTICK_Y);}
 bool pressed(){return digitalRead(JOYSTICK_SW)==LOW;}
};
EBOS_Input Input;
