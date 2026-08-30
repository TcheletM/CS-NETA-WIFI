#pragma once
#include <Arduino.h>

struct ImuData {
  float accelX, accelY, accelZ;
  float gyroX, gyroY, gyroZ;
  float tempC;
};

bool imuInit();
ImuData imuRead();
