#pragma once
#include <Arduino.h>

class EBOS_PixelEngine {
public:
 byte frame[8]={0};
 void clear(){for(byte i=0;i<8;i++)frame[i]=0;}
 void setPixel(byte x,byte y,bool on){if(x>7||y>7)return;if(on)frame[y]|=(1<<x);else frame[y]&=~(1<<x);}
 byte* getFrame(){return frame;}
};
EBOS_PixelEngine Pixel;
