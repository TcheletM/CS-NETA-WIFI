//================== BB-functionsOfButtons ==================
bool lastPlusState  = HIGH;
bool lastMinusState = HIGH;

void initButtons() {
  pinMode(BTN_PLUS_PIN, INPUT);   // פול-אפ חיצוני קיים בחומרה (R3)
  pinMode(BTN_MINUS_PIN, INPUT);  // פול-אפ חיצוני קיים בחומרה (R2)
}

void updateCounterFromButtons() {
  bool plusState  = digitalRead(BTN_PLUS_PIN);
  bool minusState = digitalRead(BTN_MINUS_PIN);

  // active LOW - מגיב רק על רגע הלחיצה (מעבר HIGH->LOW), לא כל עוד מוחזק
  if (plusState == LOW && lastPlusState == HIGH) {
    counter++;
  }
  if (minusState == LOW && lastMinusState == HIGH) {
    counter--;
  }

  lastPlusState  = plusState;
  lastMinusState = minusState;
}
