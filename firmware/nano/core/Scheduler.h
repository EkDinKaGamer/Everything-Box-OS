#pragma once

#include <Arduino.h>

struct EBOSTask {
  void (*callback)();
  unsigned long interval;
  unsigned long lastRun;
};

class EBOSScheduler {
public:
  void run(EBOSTask &task) {
    if (millis() - task.lastRun >= task.interval) {
      task.lastRun = millis();
      task.callback();
    }
  }
};

EBOSScheduler Scheduler;
