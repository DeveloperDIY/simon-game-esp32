struct button {
  const uint8_t PIN;
  byte LED;
  bool pressed;
  unsigned long pressed_time;  
  unsigned long last_pressed_time; 
};

extern button StartButton = { BUTTON_START_PIN };
extern button RedButton = { BUTTON_RED_PIN, 0 };
extern button GreenButton = { BUTTON_GREEN_PIN, 1 };
extern button BlueButton = { BUTTON_BLUE_PIN, 2 };
extern button YellowButton = { BUTTON_YELLOW_PIN, 3 };
