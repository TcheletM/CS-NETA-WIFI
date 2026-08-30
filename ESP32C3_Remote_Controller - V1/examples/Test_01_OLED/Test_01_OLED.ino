//================== Test_01_OLED ===================
#include <Wire.h>
#include <U8g2lib.h>

// OLED 0.42" – SSD1306 72x40
U8G2_SSD1306_72X40_ER_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

const int SDA_PIN = 5;
const int SCL_PIN = 6;
const int LED_PIN = 2;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  initOled();
}

uint16_t counter = 0;

void loop() {
  digitalWrite(LED_PIN, !digitalRead(LED_PIN)); // הבהוב לד על הדק 8

  display.clearBuffer();
  display.setFont(u8g2_font_6x10_tf);
  display.drawStr(0, 24, ("Count: " + String(counter++)).c_str());
  display.sendBuffer();

  Serial.print("counter=");
  Serial.println(counter);

  delay(500);
}
