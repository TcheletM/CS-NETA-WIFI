/*
  ESP32C3_Remote_Controller
  פרויקט מאוחד המפעיל את כל יחידות החומרה על הלוח:
  OLED 0.42", MPU6050, ג'ויסטיק, מנוע רטט, 4 לדים RGB, כפתורים SW_LEFT/SW_RIGHT

  כל מודול חומרה מחולק לזוג קבצים משלו (h/cpp) - ראו את שאר
  הכרטיסיות בפרויקט הזה ב-Arduino IDE.

  לפני שמריצים את הקובץ הזה - כדאי לוודא שכל רכיב עובד בנפרד
  דרך הסקצ'ים ב-examples/Test_01..Test_06.
*/
#include "Pins.h"
#include "OLED_Display.h"
#include "IMU_MPU6050.h"
#include "Joystick.h"
#include "VibrationMotor.h"
#include "RGB_LEDs.h"
#include "Buttons.h"

void setup() {
  Serial.begin(115200);

  oledInit();      // גם מאתחל את ה-I2C (Wire.begin) - חייב לרוץ ראשון
  imuInit();
  joystickInit();
  motorInit();
  ledsInit();
  buttonsInit();

  oledClear();
  oledPrintLine(0, "Remote Ready");
  oledShow();
}

void loop() {
  JoystickData joy = joystickRead();
  ImuData imu = imuRead();
  bool left  = buttonLeftPressed();
  bool right = buttonRightPressed();

  // רטט כשלוחצים על הג'ויסטיק
  if (joy.pressed) motorOn();
  else motorOff();

  // לדים: אדום = שמאל, ירוק = ימין, כחול עמום = מנוחה
  if (left)       ledsSetAll(255, 0, 0);
  else if (right) ledsSetAll(0, 255, 0);
  else            ledsSetAll(0, 0, 30);
  ledsShow();

  // תצוגת מצב על המסך
  oledClear();
  oledPrintLine(0, "X:" + String(joy.x) + " Y:" + String(joy.y));
  oledPrintLine(1, "AccZ:" + String(imu.accelZ, 1));
  oledPrintLine(2, String(left ? "L" : "-") + " " + String(right ? "R" : "-"));
  oledShow();

  delay(100);
}
