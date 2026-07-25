#pragma once
#include <Arduino.h>

class EBOS_Display {
public:
 void begin(){ }
 void bootAnimation(){ Serial.println("EBOS Display Boot"); }
 void showText(const char* text){ Serial.println(text); }
 void showIcon(byte icon){ Serial.print("ICON: "); Serial.println(icon); }
 void setBrightness(byte level){ Serial.print("Brightness: "); Serial.println(level); }
};

EBOS_Display Display;
