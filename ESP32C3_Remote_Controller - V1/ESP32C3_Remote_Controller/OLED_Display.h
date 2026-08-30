#pragma once
#include <Arduino.h>

void oledInit();
void oledClear();
void oledPrintLine(uint8_t line, const String &text);
void oledShow();
