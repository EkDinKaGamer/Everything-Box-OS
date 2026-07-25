#pragma once
#include <Arduino.h>

class EBOS_Games {
public:
 void menu(){Serial.println("Games: Snake, Pong, Reaction, Dice");}
 void snake(){Serial.println("Snake started");}
 void pong(){Serial.println("Pong started");}
 void reaction(){Serial.println("Reaction game started");}
 void dice(){Serial.println("Dice roll");}
};

EBOS_Games Games;
