#pragma once
#include <Arduino.h>

class EBOS_Utilities {
public:
 unsigned long timerStart=0;
 bool running=false;
 void coinFlip(){Serial.println(random(0,2)?"HEADS":"TAILS");}
 int randomNumber(int max){return random(0,max+1);}
 void startTimer(){timerStart=millis();running=true;}
 unsigned long stopwatch(){return running?millis()-timerStart:0;}
 void stopTimer(){running=false;}
 void dice(){Serial.print("Dice: ");Serial.println(random(1,7));}
};
EBOS_Utilities Utilities;
