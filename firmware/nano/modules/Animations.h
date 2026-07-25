#pragma once
#include <Arduino.h>

class EBOS_Animations {
public:
 void boot(){Serial.println("BOOT ANIMATION");}
 void smile(){Serial.println("SMILE ICON");}
 void heart(){Serial.println("HEART ICON");}
 void loading(){Serial.println("LOADING");}
};
EBOS_Animations Animations;
