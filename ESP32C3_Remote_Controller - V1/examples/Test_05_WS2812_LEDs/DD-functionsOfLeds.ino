//================== DD-functionsOfLeds ==================
void initLeds() {
  strip.begin();
  strip.setBrightness(50);
  strip.show();
}

// מיפוי כיוון -> צבע:
// לד 0 = שמאלה (אדום), לד 1 = ימינה (כחול)
// לד 2 = למעלה (ירוק), לד 3 = למטה (צהוב)
// בתוך האזור המת סביב המרכז - כל הלדים כבויים.
void updateDirectionalLeds(int x, int y) {
  strip.clear();

  if (x < CENTER - DEADZONE) {
    strip.setPixelColor(0, strip.Color(255, 0, 0));   // שמאלה - אדום (לד ראשון)
  } else if (x > CENTER + DEADZONE) {
    strip.setPixelColor(1, strip.Color(0, 0, 255));   // ימינה - כחול
  }

  if (y > CENTER + DEADZONE) {
    strip.setPixelColor(2, strip.Color(0, 255, 0));   // למעלה - ירוק
  } else if (y < CENTER - DEADZONE) {
    strip.setPixelColor(3, strip.Color(255, 255, 0)); // למטה - צהוב
  }

  strip.show();
}
