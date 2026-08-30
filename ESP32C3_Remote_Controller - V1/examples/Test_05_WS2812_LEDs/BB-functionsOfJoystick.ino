//================== BB-functionsOfJoystick ==================
void initJoystick() {
  analogSetPinAttenuation(PIN_JOY_VRX, ADC_11db);
  analogSetPinAttenuation(PIN_JOY_VRY, ADC_11db);
}

bool isOutOfBounds(int x, int y) {
  return (x < AXIS_MIN || x > AXIS_MAX || y < AXIS_MIN || y > AXIS_MAX);
}
