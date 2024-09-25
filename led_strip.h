#ifndef LED_STRIP_NUM_LEDS

#include "FastLED.h"

#define LED_STRIP_NUM_LEDS 12
#define LED_STRIP_DATA_PIN 38

uint8_t paletteIndex = 0;

CRGBPalette16 purplePalette(
  CRGB::DarkViolet,
  CRGB::DarkViolet,
  CRGB::DarkViolet,
  CRGB::DarkViolet,

  CRGB::Magenta,
  CRGB::Magenta,
  CRGB::DarkBlue,
  CRGB::DarkBlue,

  CRGB::Magenta,
  CRGB::Magenta,
  CRGB::DarkViolet,
  CRGB::DarkViolet,

  CRGB::DarkViolet,
  CRGB::DarkViolet,
  CRGB::DarkBlue,
  CRGB::DarkBlue
);

CRGB strip_leds[LED_STRIP_NUM_LEDS];

void ledStripShowColour(CRGB colour) {
  fill_solid( strip_leds, LED_STRIP_NUM_LEDS, colour);
  FastLED.show(); 
}

void clearLedStrip() {
  ledStripShowColour(CRGB::Black);
}

void ledStripStrobeColour(CRGB colour, int duration_ms) {
  unsigned long start_time = millis();

  while (millis() < start_time + duration_ms) {
    ledStripShowColour(colour);
    delay(100);
    clearLedStrip();
    delay(100);
  }
}

void ledStripParty() {
  EVERY_N_MILLISECONDS(50) {
    uint8_t sinBeat = beatsin8(40, 0, LED_STRIP_NUM_LEDS - 1, 0, 0);
    uint8_t sinBeat2 = beatsin8(40, 0, LED_STRIP_NUM_LEDS - 1, 0, 85);
    uint8_t sinBeat3 = beatsin8(40, 0, LED_STRIP_NUM_LEDS - 1, 0, 170);
    uint8_t sinBeat4 = beatsin8(30, 0, LED_STRIP_NUM_LEDS - 1, 0, 110);

    strip_leds[sinBeat] = CRGB::Blue;
    strip_leds[sinBeat2] = CRGB::Red;
    strip_leds[sinBeat3] = CRGB::Green;
    strip_leds[sinBeat4] = CRGB::Yellow;

    fadeToBlackBy(strip_leds, LED_STRIP_NUM_LEDS, 6);    
  }
  FastLED.show();
}

void ledStripPartyDelay(int duration_ms) {
  unsigned long start_time = millis();

  while (millis() < start_time + duration_ms) {
    ledStripParty();
  }

  clearLedStrip();
}

void ledStripSoftSparkle() {
  EVERY_N_MILLISECONDS(50) {
    strip_leds[random8(0, LED_STRIP_NUM_LEDS - 1)] = ColorFromPalette(purplePalette, random8(), 255, LINEARBLEND);
  }
  fadeToBlackBy(strip_leds, LED_STRIP_NUM_LEDS, 1);
  FastLED.show();
}

void initialiseLedStrip() {
  FastLED.addLeds<WS2811, LED_STRIP_DATA_PIN, BRG>(strip_leds, LED_STRIP_NUM_LEDS);
  clearLedStrip();
}

#endif  // LED_STRIP_NUM_LEDS