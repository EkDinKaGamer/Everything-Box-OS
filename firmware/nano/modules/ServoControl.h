#pragma once
#include <Servo.h>
#include "../Config.h"

class EBOS_Servo {
 Servo servo;
public:
 void begin(){servo.attach(SERVO_PIN);}
 void write(int angle){servo.write(angle);}
};
EBOS_Servo ServoControl;
