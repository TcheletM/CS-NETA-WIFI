//================== Test_02_ButtonCounter ===================
#include <Wire.h>
#include <U8g2lib.h>

// OLED 0.42" – SSD1306 72x40
U8G2_SSD1306_72X40_ER_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

const int SDA_PIN = 5;
const int SCL_PIN = 6;

const int BTN_PLUS_PIN  = 9;  // SW_RIGHT בלוח - מעלה את המונה
const int BTN_MINUS_PIN = 0;  // SW_LEFT בלוח - מוריד את המונה

int counter = 0;

// ----------- prototypes (הכרחי לחלוקה לטאבים) -----------
void initOled();
void drawCounter();
void initButtons();
void updateCounterFromButtons();

void setup() {
  initOled();
  initButtons();
  drawCounter();
}

void loop() {
  updateCounterFromButtons();
  drawCounter();
  delay(150);
}
