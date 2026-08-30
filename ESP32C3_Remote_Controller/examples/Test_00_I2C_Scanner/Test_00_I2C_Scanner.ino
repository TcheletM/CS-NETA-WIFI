/*
  Test 0 - סורק I2C
  בודק בפועל אילו פינים מריצים את ה-I2C של המסך, במקום להמר.
  מנסה שני זוגות פינים מועמדים: (5,6) ו-(8,9), וסורק כתובות בכל אחד.

  תוצאה צפויה למסך SSD1306: כתובת 0x3C (או 0x3D).
*/
#include <Wire.h>

struct PinPair { int sda; int scl; const char* label; };
PinPair candidates[] = {
  {8, 9, "SDA=8 SCL=9 (לפי תמונת הפינאאוט)"},
  {5, 6, "SDA=5 SCL=6 (הניחוש המקורי)"},
};

void scanPins(int sda, int scl, const char* label) {
  Serial.println();
  Serial.print("--- סורק עם ");
  Serial.println(label);

  Wire.end();          // מנתק בוס קודם אם היה פעיל
  Wire.begin(sda, scl);
  delay(50);

  int found = 0;
  for (uint8_t addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print("  נמצא מכשיר בכתובת 0x");
      Serial.println(addr, HEX);
      found++;
    }
  }
  if (found == 0) Serial.println("  לא נמצא כלום על הזוג הזה.");
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("=== סורק I2C - בדיקת חיווט OLED ===");

  for (auto &c : candidates) {
    scanPins(c.sda, c.scl, c.label);
  }

  Serial.println();
  Serial.println("סיום. איפה שנמצאה כתובת 0x3C/0x3D - אלה הפינים הנכונים למסך.");
}

void loop() {
  delay(5000);
}
