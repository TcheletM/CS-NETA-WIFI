//================== AA-functionsOfOled ==================
void initOled() {
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setTimeOut(20);
  display.begin();
  Wire.setClock(400000); // האצת קצב - 400kHz
}

void drawStatus(int x, int y, bool motorOn) {
  display.clearBuffer();
  display.setFont(u8g2_font_6x10_tf);

  display.drawStr(0, 10, ("X: " + String(x)).c_str());
  display.drawStr(0, 22, ("Y: " + String(y)).c_str());
  // הפונט הרגיל של u8g2 לא כולל עברית - הטקסט על המסך נשאר באנגלית
  display.drawStr(0, 34, motorOn ? "MOTOR: ON" : "MOTOR: OFF");

  display.sendBuffer();
}
