#pragma once
#include <Arduino.h>

class EBOS_Menu {
 int page=0;
public:
 void next(){page++; if(page>5) page=0;}
 void previous(){page--; if(page<0) page=5;}
 int current(){return page;}
 void render(){Serial.print("Menu page: ");Serial.println(page);}
};

EBOS_Menu Menu;
