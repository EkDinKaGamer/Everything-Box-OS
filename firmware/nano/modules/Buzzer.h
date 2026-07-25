#pragma once
#include <Arduino.h>
#include "../Config.h"

class EBOS_Buzzer {
public:
 void begin(){pinMode(BUZZER_PIN,OUTPUT);}
 void beep(int ms=100){digitalWrite(BUZZER_PIN,HIGH);delay(ms);digitalWrite(BUZZER_PIN,LOW);}
 void on(){digitalWrite(BUZZER_PIN,HIGH);}
 void off(){digitalWrite(BUZZER_PIN,LOW);}
};
EBOS_Buzzer Buzzer;
