// Game credits logic + coin acceptor
// Author: @DeveloperDIY
//
byte credits = 1;

byte getCredits() {
  return credits;
}

bool playerHasCredits() {
  return (credits > 0);
}

bool playerHasNoCreditsLeft() {
  return (credits == 0);
}

void addCredit(byte additional_credits) {
  credits = credits + additional_credits;
  Serial.printf("Credit added, user now has %u credits.\n", credits);
}

void useCredit(byte number_of_credits_to_use) {

  if (credits == 0) {
    Serial.printf("User has no credits left.\n");
    return;
  }

  credits = credits - number_of_credits_to_use;
  Serial.printf("Credit used, user has %u credits left.\n", credits);
}
