/*
  Everything Box OS v1.0
  Arduino Nano Main Firmware
*/

#include "Config.h"
#include "core/Scheduler.h"
#include "core/Diagnostics.h"
#include "modules/Display.h"
#include "modules/Input.h"
#include "modules/RGB.h"
#include "modules/Buzzer.h"
#include "modules/ServoControl.h"

void systemTask();

EBOSTask systemTaskItem = {systemTask, 1000, 0};

void setup() {
  Serial.begin(9600);

  Display.begin();
  Input.begin();
  RGB.begin();
  Buzzer.begin();
  ServoControl.begin();

  Diagnostics.bootCheck();
  Display.bootAnimation();

  Buzzer.beep(80);
  Serial.println("EBOS v1.0 Started");
}

void loop() {
  Scheduler.run(systemTaskItem);

  if (Input.pressed()) {
    RGB.blue();
  }
}

void systemTask() {
  Serial.println("EBOS System Running");
}
