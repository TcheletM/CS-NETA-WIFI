//================== Test_04_VibrationMotor ===================
#include <Wire.h>
#include <U8g2lib.h>

// OLED 0.42" – SSD1306 72x40
U8G2_SSD1306_72X40_ER_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

const int SDA_PIN = 5;
const int SCL_PIN = 6;

const int PIN_JOY_VRY = 3;
const int PIN_JOY_VRX = 4;

const int PIN_MOTOR = 10;   // דרך MOSFET Q1, HIGH = מנוע דולק

// תחומי הג'ויסטיק - ADC 12-bit (0-4095), מרכז בערך 2048.
// מחוץ לתחום הזה (קרוב לקצה) = המנוע נדלק כמשוב רטט.
const int AXIS_MIN = 500;
const int AXIS_MAX = 3500;

// ----------- prototypes (הכרחי לחלוקה לטאבים) -----------
void initOled();
void drawStatus(int x, int y, bool motorOn);
void initJoystick();
bool isOutOfBounds(int x, int y);
void initMotor();
void setMotor(bool on);

void setup() {
  initOled();
  initJoystick();
  initMotor();
}

void loop() {
  int vrx = analogRead(PIN_JOY_VRX);
  int vry = analogRead(PIN_JOY_VRY);

  bool outOfBounds = isOutOfBounds(vrx, vry);
  setMotor(outOfBounds);

  drawStatus(vrx, vry, outOfBounds);

  delay(100);
}
