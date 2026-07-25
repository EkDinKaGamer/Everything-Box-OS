#pragma once
#include <Arduino.h>
#include "../Config.h"

class EBOS_RGB {
public:
 void begin(){pinMode(RGB_R_PIN,OUTPUT);pinMode(RGB_G_PIN,OUTPUT);pinMode(RGB_B_PIN,OUTPUT);off();}
 void set(byte r,byte g,byte b){analogWrite(RGB_R_PIN,r);analogWrite(RGB_G_PIN,g);analogWrite(RGB_B_PIN,b);}
 void off(){set(0,0,0);}
 void red(){set(255,0,0);}
 void green(){set(0,255,0);}
 void blue(){set(0,0,255);}
};
EBOS_RGB RGB;
