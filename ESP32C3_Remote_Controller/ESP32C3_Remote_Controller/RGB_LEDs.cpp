#include "RGB_LEDs.h"
#include "Pins.h"
#include <Adafruit_NeoPixel.h>

static Adafruit_NeoPixel strip(NUM_LEDS, PIN_LEDS, NEO_GRB + NEO_KHZ800);

void ledsInit() {
  strip.begin();
  strip.setBrightness(50);
  strip.show();
}

void ledsSetAll(uint8_t r, uint8_t g, uint8_t b) {
  strip.fill(strip.Color(r, g, b));
}

void ledsSetPixel(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
  strip.setPixelColor(index, strip.Color(r, g, b));
}

void ledsShow()  { strip.show(); }
void ledsClear() { strip.clear(); }
