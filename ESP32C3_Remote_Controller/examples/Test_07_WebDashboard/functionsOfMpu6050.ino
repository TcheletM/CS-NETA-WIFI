//================= functionsOfMpu6050 ===================
// כל מה שקשור לחיישן ה-MPU6050: קריאה ישירה מהרגיסטרים (בלי ספרייה),
// החלקת הטיה על ציר X, וזיהוי "מכה" (jerk) על ציר Z.

#define MPU_ADDR 0x68

// משתני החלקה/זיהוי פנימיים של החיישן - לא קשורים לרשת/תצוגה
float smoothedTilt = 0.0;
const float ALPHA = 0.2;
int16_t last_raw_Z = 0;
const int JERK_THRESHOLD = 7000;

void initMpu6050() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
}

// קורא הטיה מוחלקת (X) וזיהוי מכה (Z) - מוגן מקריסות I2C.
// נקרא בכל לולאה, אבל בפועל קורא מהחיישן רק פעם ב-500ms; בין קריאה
// לקריאה currentTilt/specialAttack נשארים כמו שהועברו (הערכים האחרונים).
void readMpu6050(int &currentTilt, bool &specialAttack) {
  static unsigned long lastSensorRead = 0;
  if (millis() - lastSensorRead <= 500) return;
  lastSensorRead = millis();

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  byte i2cError = Wire.endTransmission(false); // בדיקה האם הקו פנוי ותקין

  if (i2cError == 0) {
    // החיישן מגיב, אפשר לקרוא בבטחה
    if (Wire.requestFrom(MPU_ADDR, 6, true) == 6) {
      int16_t AcX = Wire.read() << 8 | Wire.read();
      int16_t AcY = Wire.read() << 8 | Wire.read();
      int16_t AcZ = Wire.read() << 8 | Wire.read();

      smoothedTilt = (ALPHA * AcX) + ((1.0 - ALPHA) * smoothedTilt);
      currentTilt = (int)smoothedTilt;

      int jerk_Z = AcZ - last_raw_Z;
      last_raw_Z = AcZ;
      specialAttack = (abs(jerk_Z) > JERK_THRESHOLD);
    }
  } else {
    // החיישן נותק או שהקו ננעל, מבצעים ריסט חומרתי ל-I2C
    Serial.println("I2C Bus Lockup Detected! Resetting Hardware...");

    Wire.end(); // כיבוי מוחלט של פרוטוקול I2C
    delay(5);

    Wire.begin(SDA_PIN, SCL_PIN); // הדלקה מחדש
    Wire.setTimeOut(20);
    Wire.setClock(400000);

    // שליחת פקודת התעוררות מחדש ל-MPU6050 למקרה שאיבד חשמל
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);
  }
}
//==========================================================
