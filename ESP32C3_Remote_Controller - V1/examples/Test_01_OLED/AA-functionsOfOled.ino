//================== AA-functionsOfOled ==================
void initOled() {
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setTimeOut(20);
  display.begin();

  Wire.setClock(400000); // האצת קצב לפי הנתונים - 400kHz
  display.clearBuffer();
  display.setFont(u8g2_font_6x10_tf);
  display.drawStr(0, 12, "OLED 0.42 OK");
  display.sendBuffer();
}
