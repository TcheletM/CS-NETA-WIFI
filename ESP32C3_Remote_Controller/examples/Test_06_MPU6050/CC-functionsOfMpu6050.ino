//================== CC-functionsOfMpu6050 ==================
void initMpu6050() {
  // ה-I2C כבר אותחל ב-initOled() (Wire.begin(SDA_PIN, SCL_PIN))
  if (!mpu.begin()) {
    Serial.println("MPU6050 לא נמצא! בדוק חיווט (VCC=3.3V, SDA=IO5, SCL=IO6)");
    while (true) delay(1000);
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void readImu(float &ax, float &ay, float &gx, float &gy, float &gz) {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  ax = a.acceleration.x;
  ay = a.acceleration.y;
  gx = g.gyro.x;
  gy = g.gyro.y;
  gz = g.gyro.z;
}
