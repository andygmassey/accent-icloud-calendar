#ifndef POWER_H
#define POWER_H

#include <Arduino.h>

class Power {
 public:
  void DeepSleep(uint64_t delay_ms);
  void Restart();
 private:
  void ApplyConfigs();
};

#endif  // POWER_H
