struct button {
  const uint8_t PIN;
  byte LED;
  CRGB colour;
  bool pressed;
  unsigned long pressed_time;  
  unsigned long last_pressed_time; 
};

extern button StartButton = { BUTTON_START_PIN };
extern button RedButton = { BUTTON_RED_PIN, 0, CRGB::Red };
extern button GreenButton = { BUTTON_GREEN_PIN, 1, CRGB::Green };
extern button BlueButton = { BUTTON_BLUE_PIN, 2, CRGB::Blue };
extern button YellowButton = { BUTTON_YELLOW_PIN, 3, CRGB::Yellow };
