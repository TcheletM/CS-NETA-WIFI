//================== CC-functionsOfMotor ==================
void initMotor() {
  pinMode(PIN_MOTOR, OUTPUT);
  digitalWrite(PIN_MOTOR, LOW);
}

void setMotor(bool on) {
  digitalWrite(PIN_MOTOR, on ? HIGH : LOW);
}
