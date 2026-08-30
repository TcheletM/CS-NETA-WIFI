//================== Test_06_MPU6050 ===================
#include <Wire.h>
#include <U8g2lib.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_NeoPixel.h>

// OLED 0.42" – SSD1306 72x40
U8G2_SSD1306_72X40_ER_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

const int SDA_PIN = 5;
const int SCL_PIN = 6;

Adafruit_MPU6050 mpu;

const int PIN_LEDS = 2;
const int NUM_LEDS = 5;
Adafruit_NeoPixel strip(NUM_LEDS, PIN_LEDS, NEO_GRB + NEO_KHZ800);

// סף הטיה (מ/שנייה בריבוע) שמעליו לד הכיוון נדלק - כמו ה-DEADZONE בתרגיל 5,
// רק שכאן זה על ערכי תאוצה של ה-MPU6050 ולא על ערכי ADC של הג'ויסטיק.
const float TILT_THRESHOLD = 3.0;

// ----------- prototypes (הכרחי לחלוקה לטאבים) -----------
void initOled();
void drawGyro(float gx, float gy, float gz);
void initLeds();
void updateDirectionalLeds(float ax, float ay);
void initMpu6050();
void readImu(float &ax, float &ay, float &gx, float &gy, float &gz);

void setup() {
  initOled();
  initLeds();
  initMpu6050();
}

void loop() {
  float ax, ay, gx, gy, gz;
  readImu(ax, ay, gx, gy, gz);

  updateDirectionalLeds(ax, ay);
  drawGyro(gx, gy, gz);

  delay(100);
}
