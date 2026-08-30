//================== BB-functionsOfLeds ==================
void initLeds() {
  strip.begin();
  strip.setBrightness(50);
  strip.show();
}

// מיפוי הטיה -> צבע (אותה שיטה כמו בתרגיל 5, רק לפי תאוצת ה-MPU6050):
// לד 0 = הטיה שמאלה (אדום), לד 1 = הטיה ימינה (כחול)
// לד 2 = הטיה קדימה (ירוק), לד 3 = הטיה אחורה (צהוב)
void updateDirectionalLeds(float ax, float ay) {
  strip.clear();

  if (ax < -TILT_THRESHOLD) {
    strip.setPixelColor(0, strip.Color(255, 0, 0));   // שמאלה - אדום
  } else if (ax > TILT_THRESHOLD) {
    strip.setPixelColor(1, strip.Color(0, 0, 255));   // ימינה - כחול
  }

  if (ay > TILT_THRESHOLD) {
    strip.setPixelColor(2, strip.Color(0, 255, 0));   // קדימה - ירוק
  } else if (ay < -TILT_THRESHOLD) {
    strip.setPixelColor(3, strip.Color(255, 255, 0)); // אחורה - צהוב
  }

  strip.show();
}
