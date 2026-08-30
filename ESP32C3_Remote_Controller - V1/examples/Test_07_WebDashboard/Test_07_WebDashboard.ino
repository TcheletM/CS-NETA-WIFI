#include <WiFi.h>
#include <esp_wifi.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <WebServer.h>
#include <LittleFS.h>
#include <WebSocketsServer.h>
#include <Adafruit_NeoPixel.h>

// מצב לוח יחיד: השלט עצמו משדר את הרשת הזו (AP), במקום להתחבר לקונסולה חיצונית
const char* ssid = "ESP32_Console_V2";
const char* password = "87654321";

// OLED 0.42" – SSD1306 72x40
U8G2_SSD1306_72X40_ER_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

WebServer        httpServer(80);
WebSocketsServer wsServer(81);

// פינים - I2C (מסך + MPU6050)
#define SDA_PIN 5
#define SCL_PIN 6

// פינים - ג'ויסטיק ו-SELECT
#define VRX_PIN 4
#define VRY_PIN 3
#define SW_PIN  7

// פינים - כפתורים דיגיטליים
#define LEFT_PIN 0    // SW_LEFT
#define RIGHT_PIN 9   // SW_RIGHT

#define MOTOR_PIN 10  // תוקן לפי הסכמה החדשה (MOTOR_PIN=IO10)

// פין סוללה
#define BATTERY_PIN 1  // ADC - מחלק מתח (R17=100k, R18=100k)
const float BATTERY_CALIBRATION = 0.906;  // Correction factor - תיקנו מדי פעם

// 5 לדים RGB (WS2812B) בשרשור
#define LEDS_PIN 2
#define NUM_LEDS 5
Adafruit_NeoPixel strip(NUM_LEDS, LEDS_PIN, NEO_GRB + NEO_KHZ800);

// מנוע/לדים מגיבים לאירוע ג'ויסטיק (רגע הלחיצה לקצה), לא להטיה -
// הפעלה לפי הטיה גרמה למנוע לנער את חיישן ה-MPU ויצרה משוב עצמי.
// ledState: 0=כבוי, 1=שמאלה (אדום), 2=ימינה (כחול)
const unsigned long MOTOR_PULSE_MS = 120; // כמה זמן המנוע רוטט בכל לחיצה
const unsigned long LED_PULSE_MS   = 350; // כמה זמן הלד דולק בכל לחיצה

// --- משתני שידור ומניעת הצפת רשת ---
// btn = כפתורים פיזיים (KEY1/KEY2) בלבד. joy = ג'ויסטיק בלבד. הפרדה מוחלטת.
bool lastBtnLeft = false;
bool lastBtnRight = false;
bool lastJoyLeft = false;
bool lastJoyRight = false;
bool lastJoyUp = false;
bool lastJoyDown = false;
bool lastSelect = false;
int lastTilt = 0;
bool lastAttack = false;
bool lastMotor = false;
uint8_t lastLed = 0;
float lastBatteryVoltage = 0.0;

void setup() {
  Serial.begin(115200);
  delay(2000); 
  
  pinMode(LEFT_PIN, INPUT);
  pinMode(RIGHT_PIN, INPUT);
  pinMode(SW_PIN, INPUT_PULLUP);

  pinMode(MOTOR_PIN, OUTPUT);
  digitalWrite(MOTOR_PIN, LOW);

  strip.begin();
  strip.setBrightness(50);
  strip.show();

  initOled();

  Wire.setTimeOut(20);
  initMpu6050();

  initAP();
  initWebServer();
  initWebSocket();
}

void loop() {
  httpServer.handleClient();
  wsServer.loop();

  // --- 1. קריאת כפתורים דיגיטליים (KEY1/KEY2) - עצמאי לגמרי מהג'ויסטיק ---
  bool btnLeft = (digitalRead(LEFT_PIN) == LOW);
  bool btnRight = (digitalRead(RIGHT_PIN) == LOW);
  bool btnSelect = (digitalRead(SW_PIN) == LOW);

  // --- 2. קריאת ג'ויסטיק - עצמאי לגמרי מהכפתורים ---
  // כיוונים הפוכים לחיווט בפועל (נבדק מול החומרה): joyX/joyY הפוכים ביחס למקור.
  int joyX = analogRead(VRX_PIN);
  int joyY = analogRead(VRY_PIN);

  bool joyLeft  = (joyX > 4000);
  bool joyRight = (joyX < 100);
  bool joyUp    = (joyY > 4000);
  bool joyDown  = (joyY < 100);

  // --- 3. קריאת חיישן MPU6050 (הטיה + זיהוי מכה) - הכל ב-functionsOfMpu6050.ino ---
  int currentTilt = lastTilt;
  bool specialAttack = lastAttack;
  readMpu6050(currentTilt, specialAttack);

  // --- 3.5 משוב מקומי (מנוע + לדים) - לפי אירוע ג'ויסטיק, לא הטיה ---
  // רק ברגע שהידית עוברת לקצה (לא כל עוד מוחזקת) מתחיל פולס קצר.
  static bool prevJoyLeft = false, prevJoyRight = false, prevJoyUp = false, prevJoyDown = false;
  static unsigned long motorPulseUntil = 0;
  static unsigned long ledPulseUntil = 0;
  static uint8_t pulseLedColor = 0;

  bool joyEdge = (joyLeft && !prevJoyLeft) || (joyRight && !prevJoyRight) ||
                 (joyUp && !prevJoyUp) || (joyDown && !prevJoyDown);

  // רק ג'ויסטיק מפעיל את המנוע - לא specialAttack (זה זיהוי טלטול מה-MPU,
  // כלומר תנועת לוח, בדיוק מה שלא רוצים שיפעיל את המנוע).
  if (joyEdge) {
    motorPulseUntil = millis() + MOTOR_PULSE_MS;
  }
  if (joyLeft && !prevJoyLeft) {
    pulseLedColor = 1; // שמאלה - אדום
    ledPulseUntil = millis() + LED_PULSE_MS;
  } else if (joyRight && !prevJoyRight) {
    pulseLedColor = 2; // ימינה - כחול
    ledPulseUntil = millis() + LED_PULSE_MS;
  } else if (joyUp && !prevJoyUp) {
    pulseLedColor = 3; // למעלה - ירוק
    ledPulseUntil = millis() + LED_PULSE_MS;
  } else if (joyDown && !prevJoyDown) {
    pulseLedColor = 4; // למטה - צהוב
    ledPulseUntil = millis() + LED_PULSE_MS;
  }

  prevJoyLeft = joyLeft; prevJoyRight = joyRight; prevJoyUp = joyUp; prevJoyDown = joyDown;

  bool motorOn = millis() < motorPulseUntil;
  digitalWrite(MOTOR_PIN, motorOn ? HIGH : LOW);

  uint8_t ledState = (millis() < ledPulseUntil) ? pulseLedColor : 0;

  if (ledState != lastLed) {
    if (ledState == 1)      strip.fill(strip.Color(255, 0, 0));   // שמאלה - אדום
    else if (ledState == 2) strip.fill(strip.Color(0, 0, 255));   // ימינה - כחול
    else if (ledState == 3) strip.fill(strip.Color(0, 255, 0));   // למעלה - ירוק
    else if (ledState == 4) strip.fill(strip.Color(255, 255, 0)); // למטה - צהוב
    else                    strip.clear();
    strip.show();
  }

  // --- 3.6 חיווי חי על מסך ה-OLED - בלי צורך ב-Serial Monitor ---
  drawStatus(currentTilt, joyX, joyY, btnLeft, btnRight, specialAttack, motorOn, ledState);

  // --- 3.7 קריאת מתח הסוללה ---
  int batteryRaw = analogRead(BATTERY_PIN);
  float batteryVoltage = ((batteryRaw / 4095.0) * 3.3 * 2.0) * BATTERY_CALIBRATION;

  static unsigned long lastSendTime = 0;
  static unsigned long lastBatterySendTime = 0;

if (millis() - lastSendTime >= 50) {
  // בדיקת שינויים, יצירת JSON ושליחה

  // --- 4. בדיקת שינויים ושידור ---
  if (btnLeft != lastBtnLeft || btnRight != lastBtnRight ||
      joyLeft != lastJoyLeft || joyRight != lastJoyRight ||
      joyUp != lastJoyUp || joyDown != lastJoyDown || btnSelect != lastSelect ||
      specialAttack != lastAttack || abs(currentTilt - lastTilt) > 150 ||
      motorOn != lastMotor || ledState != lastLed || abs(batteryVoltage - lastBatteryVoltage) > 0.05) {

    char jsonBuffer[250];
    snprintf(jsonBuffer, sizeof(jsonBuffer),
            "CTRL:{\"btnLeft\":%d,\"btnRight\":%d,\"joyLeft\":%d,\"joyRight\":%d,\"joyUp\":%d,\"joyDown\":%d,\"select\":%d,\"tilt\":%d,\"attack\":%d,\"motor\":%d,\"led\":%d,\"battery\":%.2f}",
            btnLeft, btnRight, joyLeft, joyRight, joyUp, joyDown, btnSelect, currentTilt, specialAttack, motorOn, ledState, batteryVoltage);

    wsServer.broadcastTXT(jsonBuffer);

    lastBtnLeft = btnLeft; lastBtnRight = btnRight;
    lastJoyLeft = joyLeft; lastJoyRight = joyRight; lastJoyUp = joyUp; lastJoyDown = joyDown;
    lastSelect = btnSelect;
    lastTilt = currentTilt;
    lastAttack = specialAttack;
    lastMotor = motorOn;
    lastLed = ledState;
    lastBatteryVoltage = batteryVoltage;
  }

  // שדור סוללה רק כשיש שינוי משמעותי (יותר מ-0.1V)
  if (abs(batteryVoltage - lastBatteryVoltage) > 0.1 || millis() - lastBatterySendTime >= 5000) {
    char batteryBuffer[100];
    snprintf(batteryBuffer, sizeof(batteryBuffer),
            "CTRL:{\"battery\":%.2f}",
            batteryVoltage);
    wsServer.broadcastTXT(batteryBuffer);
    lastBatterySendTime = millis();
  }

lastSendTime = millis();
}
  //Serial.println(" | joyX: " + String(joyX) + " | joyY: " + String(joyY));
  yield();
  delay(50); // 50 fps
}