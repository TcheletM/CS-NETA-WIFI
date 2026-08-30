#include "IMU_MPU6050.h"
#include "Pins.h"
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

static Adafruit_MPU6050 mpu;

bool imuInit() {
  // ה-I2C כבר אותחל ב-oledInit() (Wire.begin(PIN_SDA, PIN_SCL)).
  // אם רוצים להריץ את המודול הזה לבד בלי ה-OLED, יש לקרוא כאן
  // Wire.begin(PIN_SDA, PIN_SCL) לפני mpu.begin().
  if (!mpu.begin()) {
    Serial.println("שגיאה: MPU6050 לא נמצא (בדוק VCC=3.3V, SDA=IO5, SCL=IO6)");
    return false;
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  return true;
}

ImuData imuRead() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  ImuData d;
  d.accelX = a.acceleration.x;
  d.accelY = a.acceleration.y;
  d.accelZ = a.acceleration.z;
  d.gyroX  = g.gyro.x;
  d.gyroY  = g.gyro.y;
  d.gyroZ  = g.gyro.z;
  d.tempC  = temp.temperature;
  return d;
}
