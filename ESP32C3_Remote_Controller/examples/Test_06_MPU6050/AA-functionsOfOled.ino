//================== AA-functionsOfOled ==================
void initOled() {
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setTimeOut(20);
  display.begin();
  Wire.setClock(400000); // האצת קצב - 400kHz
}

void drawGyro(float gx, float gy, float gz) {
  display.clearBuffer();
  display.setFont(u8g2_font_6x10_tf);

  // הפונט הרגיל של u8g2 לא כולל עברית - הטקסט על המסך נשאר באנגלית
  display.drawStr(0, 10, ("gX: " + String(gx, 1)).c_str());
  display.drawStr(0, 22, ("gY: " + String(gy, 1)).c_str());
  display.drawStr(0, 34, ("gZ: " + String(gz, 1)).c_str());

  display.sendBuffer();
}
