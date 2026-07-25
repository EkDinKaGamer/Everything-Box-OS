#pragma once
#include "../modules/PixelEngine.h"

class EBOS_PixelDraw {
public:
 void draw(byte x,byte y){Pixel.setPixel(x,y,true);}
 void erase(byte x,byte y){Pixel.setPixel(x,y,false);}
 void clear(){Pixel.clear();}
};
EBOS_PixelDraw PixelDraw;
