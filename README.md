# ESP32C3 Remote Controller

פרויקט שלט מבוסס ESP32-C3 (לוח "ESP32-C3 0.42 OLED" מ-AliExpress) עם:
מסך OLED 0.42", MPU6050 (ג'יירו/אקסלרומטר), ג'ויסטיק אנלוגי, מנוע רטט, 5 לדים RGB (WS2812B), ושני כפתורים (SW_LEFT / SW_RIGHT).

## מבנה הפרויקט

```
ESP32C3_Remote_Controller/
├── examples/                        <- סקצ'ים לבדיקת כל רכיב בנפרד
│   ├── Test_00_I2C_Scanner/          <- כלי אבחון (לא חלק מרצף הקושי)
│   ├── Test_01_OLED/                 Test_01_OLED.ino + AA-functionsOfOled.ino
│   ├── Test_02_ButtonCounter/        Test_02_ButtonCounter.ino + AA-functionsOfOled.ino + BB-functionsOfButtons.ino
│   ├── Test_03_Joystick/             Test_03_Joystick.ino + AA-functionsOfOled.ino + BB-functionsOfJoystick.ino
│   ├── Test_04_VibrationMotor/       Test_04_VibrationMotor.ino + AA-functionsOfOled.ino + BB-functionsOfJoystick.ino + CC-functionsOfMotor.ino
│   ├── Test_05_WS2812_LEDs/          Test_05_WS2812_LEDs.ino + AA-functionsOfOled.ino + BB-functionsOfJoystick.ino + CC-functionsOfMotor.ino + DD-functionsOfLeds.ino
│   ├── Test_06_MPU6050/              Test_06_MPU6050.ino + AA-functionsOfOled.ino + BB-functionsOfLeds.ino + CC-functionsOfMpu6050.ino
│   └── Test_07_WebDashboard/         שלט מחובר לדף אינטרנט (לוח יחיד, AP+שרת+דשבורד גרפי) - עותק
│                                       של הפרויקט העצמאי, ראו README נפרד בתוך התיקייה
└── ESP32C3_Remote_Controller/        <- הפרויקט המאוחד (פותחים את זה ב-Arduino IDE)
    ├── ESP32C3_Remote_Controller.ino <- setup()/loop() ראשי
    ├── Pins.h                        <- כל הגדרות הפינים במקום אחד
    ├── OLED_Display.h / .cpp
    ├── IMU_MPU6050.h / .cpp
    ├── Joystick.h / .cpp
    ├── VibrationMotor.h / .cpp
    ├── RGB_LEDs.h / .cpp
    └── Buttons.h / .cpp
```

כל תרגיל ב-`examples/` (חוץ מהסורק) בנוי בשני טאבים באותה תיקייה, באותה
מוסכמה של הפרויקט הראשי שלך (`ESP32C3042.ino` + `functionsOfOled.ino` וכו'):
- **`<שם התרגיל>.ino`** - הטאב הראשי, בשם התיקייה עצמה (חובה לפי Arduino IDE).
  כולל את כל ה-`#include`, הצהרות אובייקטים/פינים גלובליות, פרוטוטייפים,
  וה-`setup()`/`loop()`.
- **`AA-functionsOfX.ino`** - טאב שני באותה תיקייה עם פונקציית האתחול
  (`initOled()`, `initMpu6050()` וכו'), עם קידומת `AA-` כדי שיהיה סדר קבוע
  ברשימת הטאבים גם כשיש הרבה קבצים.

## סדר עבודה מומלץ

התרגילים ב-`examples/` ממוספרים מהקל לקשה (01→06). `Test_00_I2C_Scanner`
הוא כלי אבחון ולא חלק מהרצף.

1. פותחים כל תיקיית תרגיל ב-`examples/` בנפרד ב-Arduino IDE (שני הטאבים
   באותה תיקייה ייפתחו יחד) ומעלים ללוח כדי לוודא שכל רכיב עובד לבד.
2. רק אחרי שכל הבדיקות עברו - פותחים את `ESP32C3_Remote_Controller/ESP32C3_Remote_Controller.ino` (הפרויקט המאוחד, כל הקבצים באותה תיקייה ייפתחו כטאבים ב-Arduino IDE).

## מיפוי פינים (מתוך הסכמה)

| רכיב | פונקציה | GPIO |
|---|---|---|
| OLED (מובנה בלוח) | SDA | IO5 |
| OLED (מובנה בלוח) | SCL | IO6 |
| MPU6050 (חיצוני, אותו באס I2C) | SDA | IO5 |
| MPU6050 (חיצוני, אותו באס I2C) | SCL | IO6 |
| MPU6050 | VCC / GND | 3.3V / GND |
| MPU6050 | INT | מוארק (לא בשימוש) |
| ג'ויסטיק | VRY | IO3 |
| ג'ויסטיק | VRX | IO4 |
| ג'ויסטיק | SW_SEL (לחיצה, active LOW) | IO7 |
| כפתור שמאל | SW_LEFT (active LOW) | IO0 |
| כפתור ימין | SW_RIGHT (active LOW) | IO8 |
| מנוע רטט | MOTOR_PIN (דרך Q1 MOSFET, HIGH=דולק) | IO10 |
| לדים WS2812B (x4 בשרשור) | LEDS (data in) | IO1 |
| — | פנוי/s# ESP32C3 Remote Controller

פרויקט שלט מבוסס ESP32-C3 (לוח "ESP32-C3 0.42 OLED" מ-AliExpress) עם:
מסך OLED 0.42", MPU6050 (ג'יירו/אקסלרומטר), ג'ויסטיק אנלוגי, מנוע רטט,
4 לדים RGB (WS2812B), ושני כפתורים (SW_LEFT / SW_RIGHT).

## מבנה הפרויקט

```
ESP32C3_Remote_Controller/
├── examples/                        <- סקצ'ים לבדיקת כל רכיב בנפרד
│   ├── Test_00_I2C_Scanner/          <- כלי אבחון (לא חלק מרצף הקושי)
│   ├── Test_01_OLED/                 Test_01_OLED.ino + AA-functionsOfOled.ino
│   ├── Test_02_ButtonCounter/        Test_02_ButtonCounter.ino + AA-functionsOfOled.ino + BB-functionsOfButtons.ino
│   ├── Test_03_Joystick/             Test_03_Joystick.ino + AA-functionsOfOled.ino + BB-functionsOfJoystick.ino
│   ├── Test_04_VibrationMotor/       Test_04_VibrationMotor.ino + AA-functionsOfOled.ino + BB-functionsOfJoystick.ino + CC-functionsOfMotor.ino
│   ├── Test_05_WS2812_LEDs/          Test_05_WS2812_LEDs.ino + AA-functionsOfOled.ino + BB-functionsOfJoystick.ino + CC-functionsOfMotor.ino + DD-functionsOfLeds.ino
│   ├── Test_06_MPU6050/              Test_06_MPU6050.ino + AA-functionsOfOled.ino + BB-functionsOfLeds.ino + CC-functionsOfMpu6050.ino
│   └── Test_07_WebDashboard/         שלט מחובר לדף אינטרנט (לוח יחיד, AP+שרת+דשבורד גרפי) - עותק
│                                       של הפרויקט העצמאי, ראו README נפרד בתוך התיקייה
└── ESP32C3_Remote_Controller/        <- הפרויקט המאוחד (פותחים את זה ב-Arduino IDE)
    ├── ESP32C3_Remote_Controller.ino <- setup()/loop() ראשי
    ├── Pins.h                        <- כל הגדרות הפינים במקום אחד
    ├── OLED_Display.h / .cpp
    ├── IMU_MPU6050.h / .cpp
    ├── Joystick.h / .cpp
    ├── VibrationMotor.h / .cpp
    ├── RGB_LEDs.h / .cpp
    └── Buttons.h / .cpp
```

כל תרגיל ב-`examples/` (חוץ מהסורק) בנוי בשני טאבים באותה תיקייה, באותה
מוסכמה של הפרויקט הראשי שלך (`ESP32C3042.ino` + `functionsOfOled.ino` וכו'):
- **`<שם התרגיל>.ino`** - הטאב הראשי, בשם התיקייה עצמה (חובה לפי Arduino IDE).
  כולל את כל ה-`#include`, הצהרות אובייקטים/פינים גלובליות, פרוטוטייפים,
  וה-`setup()`/`loop()`.
- **`AA-functionsOfX.ino`** - טאב שני באותה תיקייה עם פונקציית האתחול
  (`initOled()`, `initMpu6050()` וכו'), עם קידומת `AA-` כדי שיהיה סדר קבוע
  ברשימת הטאבים גם כשיש הרבה קבצים.

## סדר עבודה מומלץ

התרגילים ב-`examples/` ממוספרים מהקל לקשה (01→06). `Test_00_I2C_Scanner`
הוא כלי אבחון ולא חלק מהרצף.

1. פותחים כל תיקיית תרגיל ב-`examples/` בנפרד ב-Arduino IDE (שני הטאבים
   באותה תיקייה ייפתחו יחד) ומעלים ללוח כדי לוודא שכל רכיב עובד לבד.
2. רק אחרי שכל הבדיקות עברו - פותחים את `ESP32C3_Remote_Controller/ESP32C3_Remote_Controller.ino` (הפרויקט המאוחד, כל הקבצים באותה תיקייה ייפתחו כטאבים ב-Arduino IDE).

## מיפוי פינים (מתוך הסכמה)

| רכיב | פונקציה | GPIO |
|---|---|---|
| OLED (מובנה בלוח) | SDA | IO5 |
| OLED (מובנה בלוח) | SCL | IO6 |
| MPU6050 (חיצוני, אותו באס I2C) | SDA | IO5 |
| MPU6050 (חיצוני, אותו באס I2C) | SCL | IO6 |
| MPU6050 | VCC / GND | 3.3V / GND |
| MPU6050 | INT | מוארק (לא בשימוש) |
| ג'ויסטיק | VRY | IO3 |
| ג'ויסטיק | VRX | IO4 |
| ג'ויסטיק | SW_SEL (לחיצה, active LOW) | IO7 |
| כפתור שמאל | SW_LEFT (active LOW) | IO0 |
| כפתור ימין | SW_RIGHT (active LOW) | IO8 |
| מנוע רטט | MOTOR_PIN (דרך Q1 MOSFET, HIGH=דולק) | IO10 |
| לדים WS2812B (x4 בשרשור) | LEDS (data in) | IO1 |
| — | פנוי/strapping, לא בשימוש | IO9 |
| — | פנוי/strapping, לא בשימוש | IO2 |

כל כפתורי הלחיצה (SW_SEL, SW_LEFT, SW_RIGHT) כבר כוללים פול-אפ חיצוני
10kΩ על הלוח עצמו - אין צורך ב-`INPUT_PULLUP` בקוד, `INPUT` רגיל מספיק.

## ספריות נדרשות (Arduino IDE Library Manager)

- **U8g2** (by oliver) - למסך ה-OLED
- **Adafruit MPU6050** + **Adafruit Unified Sensor** + **Adafruit BusIO** - ל-MPU6050
- **Adafruit NeoPixel** - ללדים WS2812B

## הערות חשובות

- **Board** ב-Arduino IDE: יש לבחור לוח מסוג `ESP32C3 Dev Module` (Boards Manager: esp32 by Espressif Systems).
- מסך ה-0.42" מבוסס SSD1306 ברזולוציה 72x40 בכתובת I2C `0x3C` - הערכים המקובלים לדגם הזה, אך יש קלונים עם היסט תצוגה שונה. אם הטקסט נראה חתוך/מוזז, בדקו את דף המוצר הספציפי שלכם לגבי offset.
- IO9 ו-IO2 הם strapping pins ב-ESP32-C3 (משפיעים על מצב האתחול/בוט) - נשארו פנויים בסכמה ולכן לא הוקצתה להם פונקציה בקוד.
- `u8g2.begin()` (בניגוד ל-`Adafruit_SSD1306::begin()`) לא מחזיר הצלחה/כישלון - אי אפשר לדעת מהערך המוחזר אם המסך נמצא. אם המסך ריק בפועל, זה עלול להיראות כאילו הכל תקין בקוד. להרצת בדיקת חיווט אמיתית - יש להשתמש ב-`Test_00_I2C_Scanner` שסורק כתובות בפועל.
- **חשוב**: מסך OLED לא מוחק את עצמו אם ה-I2C מפסיק להגיב - התמונה האחרונה שהוצגה בהצלחה נשארת "קפואה" על המסך. כדי לבדוק אם המסך באמת מגיב, יש לוודא שתוכן על המסך משתנה בזמן אמת (למשל המונה ב-`Test_01_OLED/Test_01_OLED.ino`), ולנתק חשמל לגמרי בין בדיקות כדי לנקות תוכן ישן.
trapping, לא בשימוש | IO9 |
| — | פנוי/strapping, לא בשימוש | IO2 |

כל כפתורי הלחיצה (SW_SEL, SW_LEFT, SW_RIGHT) כבר כוללים פול-אפ חיצוני
10kΩ על הלוח עצמו - אין צורך ב-`INPUT_PULLUP` בקוד, `INPUT` רגיל מספיק.

## ספריות נדרשות (Arduino IDE Library Manager)

- **U8g2** (by oliver) - למסך ה-OLED
- **Adafruit MPU6050** + **Adafruit Unified Sensor** + **Adafruit BusIO** - ל-MPU6050
- **Adafruit NeoPixel** - ללדים WS2812B

## הערות חשובות

- **Board** ב-Arduino IDE: יש לבחור לוח מסוג `ESP32C3 Dev Module` (Boards Manager: esp32 by Espressif Systems).
- מסך ה-0.42" מבוסס SSD1306 ברזולוציה 72x40 בכתובת I2C `0x3C` - הערכים המקובלים לדגם הזה, אך יש קלונים עם היסט תצוגה שונה. אם הטקסט נראה חתוך/מוזז, בדקו את דף המוצר הספציפי שלכם לגבי offset.
- IO9 ו-IO2 הם strapping pins ב-ESP32-C3 (משפיעים על מצב האתחול/בוט) - נשארו פנויים בסכמה ולכן לא הוקצתה להם פונקציה בקוד.
- `u8g2.begin()` (בניגוד ל-`Adafruit_SSD1306::begin()`) לא מחזיר הצלחה/כישלון - אי אפשר לדעת מהערך המוחזר אם המסך נמצא. אם המסך ריק בפועל, זה עלול להיראות כאילו הכל תקין בקוד. להרצת בדיקת חיווט אמיתית - יש להשתמש ב-`Test_00_I2C_Scanner` שסורק כתובות בפועל.
- **חשוב**: מסך OLED לא מוחק את עצמו אם ה-I2C מפסיק להגיב - התמונה האחרונה שהוצגה בהצלחה נשארת "קפואה" על המסך. כדי לבדוק אם המסך באמת מגיב, יש לוודא שתוכן על המסך משתנה בזמן אמת (למשל המונה ב-`Test_01_OLED/Test_01_OLED.ino`), ולנתק חשמל לגמרי בין בדיקות כדי לנקות תוכן ישן.
