#ifndef LED_STRIP_NUM_LEDS

#include "FastLED.h"

#define LED_STRIP_NUM_LEDS 6
#define LED_STRIP_DATA_PIN 38

CRGB strip_leds[LED_STRIP_NUM_LEDS];

void ledStripShowColour(CRGB colour) {
  fill_solid( strip_leds, LED_STRIP_NUM_LEDS, colour);
  FastLED.show(); 
}

void clearLedStrip() {
  ledStripShowColour(CRGB::Black);
}

void ledStripStrobeColour(CRGB colour, int duration_ms) {
  CRGB show_colour = colour;
  unsigned long start_time = millis();

  while (start_time - millis() <= duration_ms) {
    ledStripShowColour(show_colour);
    delay(200);
    show_colour = (show_colour == colour) ? CRGB::Black : colour;
  }
}

void initialiseLedStrip() {
  FastLED.addLeds<WS2811, LED_STRIP_DATA_PIN, BRG>(strip_leds, LED_STRIP_NUM_LEDS);
  clearLedStrip();
}

#endif  // LED_STRIP_NUM_LEDS