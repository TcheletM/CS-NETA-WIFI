#pragma once
#include <Arduino.h>

void ledsInit();
void ledsSetAll(uint8_t r, uint8_t g, uint8_t b);
void ledsSetPixel(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
void ledsShow();
void ledsClear();
