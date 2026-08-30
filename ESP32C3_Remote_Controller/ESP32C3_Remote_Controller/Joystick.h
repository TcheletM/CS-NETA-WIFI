#pragma once
#include <Arduino.h>

struct JoystickData {
  int x;
  int y;
  bool pressed;
};

void joystickInit();
JoystickData joystickRead();
