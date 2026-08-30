#pragma once

// מיפוי פינים - ESP32-C3 0.42 OLED Remote Controller
// מבוסס על הסכמה: H1/H2 (HDR-F-2.54 1x8) מחוברים ישירות ל-U1

// I2C - משותף בין ה-OLED המובנה לבין MPU6050 החיצוני
#define PIN_SDA         5
#define PIN_SCL         6

// ג'ויסטיק (VRX/VRY אנלוגי + לחיצה)
#define PIN_JOY_VRY     3
#define PIN_JOY_VRX     4
#define PIN_JOY_SW_SEL  7   // active LOW, פול-אפ חיצוני 10k על הלוח (R1)

// כפתורים
#define PIN_SW_LEFT     0   // active LOW, פול-אפ חיצוני 10k על הלוח (R2)
#define PIN_SW_RIGHT    8   // active LOW, פול-אפ חיצוני 10k על הלוח (R3)

// מנוע רטט (Q1 MOSFET), HIGH = דולק
#define PIN_MOTOR       10

// לדים WS2812B (4 יחידות בשרשור)
#define PIN_LEDS        1
#define NUM_LEDS        4

// פינים פנויים המובאים לחיבור אך לא בשימוש בסכמה זו.
// שני אלה הם strapping pins ב-ESP32-C3 - לא מומלץ להשתמש בהם
// כפלטים דיגיטליים כלליים בלי להבין את השפעת מצב האתחול:
//   IO9 (GPIO9) - strapping pin
//   IO2 (GPIO2) - strapping pin
