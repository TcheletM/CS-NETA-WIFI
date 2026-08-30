//================== AA-functionsOfOled ==================
void initOled() {
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setTimeOut(20);
  display.begin();
  Wire.setClock(400000); // האצת קצב - 400kHz
}

void drawCounter() {
  display.clearBuffer();

  display.setFont(u8g2_font_6x10_tf);
  display.drawStr(0, 12, "Counter:");

  display.setFont(u8g2_font_logisoso16_tf); // גופן גדול לערך המונה
  display.drawStr(0, 36, String(counter).c_str());

  display.sendBuffer();
}
