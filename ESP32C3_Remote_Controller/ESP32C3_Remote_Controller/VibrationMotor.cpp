#include "VibrationMotor.h"
#include "Pins.h"

void motorInit() {
  pinMode(PIN_MOTOR, OUTPUT);
  digitalWrite(PIN_MOTOR, LOW);
}

void motorOn()  { digitalWrite(PIN_MOTOR, HIGH); }
void motorOff() { digitalWrite(PIN_MOTOR, LOW); }
