#include "Buttons.h"
#include "Pins.h"

void buttonsInit() {
  pinMode(PIN_SW_LEFT, INPUT);   // פול-אפ חיצוני קיים בחומרה (R2)
  pinMode(PIN_SW_RIGHT, INPUT);  // פול-אפ חיצוני קיים בחומרה (R3)
}

bool buttonLeftPressed()  { return digitalRead(PIN_SW_LEFT)  == LOW; }
bool buttonRightPressed() { return digitalRead(PIN_SW_RIGHT) == LOW; }
