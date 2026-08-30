#include "Joystick.h"
#include "Pins.h"

void joystickInit() {
  pinMode(PIN_JOY_SW_SEL, INPUT); // פול-אפ חיצוני קיים בחומרה (R1)
  analogSetPinAttenuation(PIN_JOY_VRX, ADC_11db);
  analogSetPinAttenuation(PIN_JOY_VRY, ADC_11db);
}

JoystickData joystickRead() {
  JoystickData d;
  d.x = analogRead(PIN_JOY_VRX);
  d.y = analogRead(PIN_JOY_VRY);
  d.pressed = (digitalRead(PIN_JOY_SW_SEL) == LOW);
  return d;
}
