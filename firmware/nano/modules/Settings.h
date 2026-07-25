#pragma once
#include <Arduino.h>

class EBOS_Settings {
public:
 byte brightness=5;
 bool sound=true;
 void setBrightness(byte b){brightness=b;}
 void toggleSound(){sound=!sound;}
 bool soundEnabled(){return sound;}
};
EBOS_Settings Settings;
