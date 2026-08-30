//================= functionsOfOled =================
void initOled() {
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setTimeOut(20);
  display.begin();

  Wire.setClock(400000); // האצת קצב העברת הנתונים לפי 4

  display.clearBuffer();
  display.setFont(u8g2_font_6x10_tf);
  display.drawStr(0, 12, "OLED 0.42 OK");
  display.sendBuffer();
}

int mapY(byte y) {
  if (y < 10) return 10;
  if (y < 20) return 20;
  if (y < 30) return 30;
  return 38;
}

void functionsForPrintingAmessage(byte x, byte y, String str) {
  display.clearBuffer();
  display.setFont(u8g2_font_6x10_tf);
  display.drawStr(x, mapY(y), str.c_str());
  display.sendBuffer();
}

// חיווי חי על המסך - כל מצב השלט בלי צורך ב-Serial Monitor
// כולל joyX/joyY הגולמיים - לכיוונון חד-משמעי של כיווני הג'ויסטיק בפועל
// (בלי לנחש: מטים לכיוון מסוים ורואים בדיוק איזה ערך המספר מציג).
void drawStatus(int tilt, int joyX, int joyY, bool left, bool right, bool attack, bool motor, uint8_t led) {
  display.clearBuffer();
  display.setFont(u8g2_font_6x10_tf);

  display.drawStr(0, 9, ("T:" + String(tilt)).c_str());

  String line2 = "X:" + String(joyX) + " Y:" + String(joyY);
  display.drawStr(0, 19, line2.c_str());

  String line3 = "L:" + String(left) + " R:" + String(right) + " A:" + String(attack);
  display.drawStr(0, 29, line3.c_str());

  String ledTxt = led == 1 ? "RED" : (led == 2 ? "BLU" : (led == 3 ? "GRN" : (led == 4 ? "YLW" : "-")));
  String line4 = "M:" + String(motor ? "ON " : "OFF") + " LED:" + ledTxt;
  display.drawStr(0, 39, line4.c_str());

  display.sendBuffer();
}
//===================================================