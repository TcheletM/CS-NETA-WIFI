#include "OLED_Display.h"
#include "Pins.h"
#include <Wire.h>
#include <U8g2lib.h>

static U8G2_SSD1306_72X40_ER_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void oledInit() {
  // חובה להריץ Wire.begin() לפני u8g2.begin() - הקונסטרוקטור F_HW_I2C
  // לא מקבל פיני SDA/SCL, הוא משתמש באובייקט Wire הגלובלי כמו שהוא.
  Wire.begin(PIN_SDA, PIN_SCL);

  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.sendBuffer();
}

void oledClear() {
  u8g2.clearBuffer();
}

void oledPrintLine(uint8_t line, const String &text) {
  // u8g2_font_6x10_tf: כל שורה כ-10px גובה, ה-y הוא ה-baseline של הטקסט
  u8g2.drawStr(0, (line + 1) * 10, text.c_str());
}

void oledShow() {
  u8g2.sendBuffer();
}
