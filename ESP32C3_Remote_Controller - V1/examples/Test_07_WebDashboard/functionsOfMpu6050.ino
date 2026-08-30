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
  // reset קשה ל-I2C first
  Wire.end();
  delay(10);
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setTimeOut(20);
  Wire.setClock(400000);
  delay(100);

  // שיחקור: האם MPU6050 מגיב
  Wire.beginTransmission(MPU_ADDR);
  byte error = Wire.endTransmission(true);

  if (error == 0) {
    // MPU מגיב - wake it up
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x6B);  // PWR_MGMT_1
    Wire.write(0x00);  // wake up
    Wire.endTransmission(true);
    delay(50);
  } else {
    Serial.print("MPU6050 Not Responding! Error: ");
    Serial.println(error);
  }
}

// קורא הטיה מוחלקת (X) וזיהוי מכה (Z) - מוגן מקריסות I2C.
// נקרא בכל לולאה, אבל בפועל קורא מהחיישן רק פעם ב-500ms; בין קריאה
// לקריאה currentTilt/specialAttack נשארים כמו שהועברו (הערכים האחרונים).
void readMpu6050(int &currentTilt, bool &specialAttack) {
  static unsigned long lastSensorRead = 0;
  if (millis() - lastSensorRead <= 50) return;  // קרא כל 50ms בשביל response טוב יותר
  lastSensorRead = millis();

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);  // ACCEL_XOUT_H
  byte i2cError = Wire.endTransmission(false);

  if (i2cError == 0) {
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
    static unsigned long lastResetTime = 0;
    if (millis() - lastResetTime > 2000) {  // reset רק כל 2 שניות לא לעזוב במעגל reset
      Serial.println("I2C Bus Error Detected! Attempting Reset...");

      Wire.end();
      delay(10);

      Wire.begin(SDA_PIN, SCL_PIN);
      Wire.setTimeOut(20);
      Wire.setClock(400000);
      delay(50);

      // wake MPU again
      Wire.beginTransmission(MPU_ADDR);
      Wire.write(0x6B);
      Wire.write(0x00);
      Wire.endTransmission(true);

      lastResetTime = millis();
    }
  }
}
//==========================================================
