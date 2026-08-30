//================== Test_03_Joystick ===================
#include <Wire.h>
#include <U8g2lib.h>

// OLED 0.42" – SSD1306 72x40
U8G2_SSD1306_72X40_ER_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

const int SDA_PIN = 5;
const int SCL_PIN = 6;

const int PIN_JOY_VRY    = 3;
const int PIN_JOY_VRX    = 4;
const int PIN_JOY_SW_SEL = 7;   // active LOW, פול-אפ חיצוני 10k קיים על הלוח (R1)

// ----------- prototypes (הכרחי לחלוקה לטאבים) -----------
void initOled();
void drawJoystick(int x, int y, bool pressed);
void initJoystick();

void setup() {
  initOled();
  initJoystick();
}

void loop() {
  int vrx = analogRead(PIN_JOY_VRX);
  int vry = analogRead(PIN_JOY_VRY);
  bool pressed = (digitalRead(PIN_JOY_SW_SEL) == LOW);

  drawJoystick(vrx, vry, pressed);

  delay(100);
}
