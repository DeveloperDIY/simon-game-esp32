#define MAX_GAME_LENGTH 4

// GPIO Pins
#define SPEAKER_PIN 47
#define BUTTON_START_PIN 37
#define COIN_ACCEPTOR_PIN 36
#define DOOR_LATCH_PIN 35

#define BUTTON_RED_PIN 7
#define BUTTON_GREEN_PIN 6
#define BUTTON_BLUE_PIN 5
#define BUTTON_YELLOW_PIN 4

#define LED_RED_PIN 18
#define LED_GREEN_PIN 17
#define LED_BLUE_PIN 16
#define LED_YELLOW_PIN 15

#include "buttons.h"
#include "pitches.h"
#include "sounds.h"
#include "game.h"
#include "game_credits.h"
#include "lcd_display.h"

// Arrays
const byte ledPins[] = { LED_RED_PIN, LED_GREEN_PIN, LED_BLUE_PIN, LED_YELLOW_PIN };
const byte buttonPins[] = { BUTTON_RED_PIN, BUTTON_GREEN_PIN, BUTTON_BLUE_PIN, BUTTON_YELLOW_PIN };
const int gameTones[] = { NOTE_G3, NOTE_C4, NOTE_E4, NOTE_G5};

struct {
  const uint8_t PIN = COIN_ACCEPTOR_PIN;
  bool coin_inserted;
  uint32_t inserted_coin_count;
  unsigned long pulse_time;  
  unsigned long last_pulse_time; 
} CoinAcceptor;

/* Global variables - store the game state */
byte gameSequence[MAX_GAME_LENGTH] = {0};
bool levelInitialised = false;
byte playerResponseSequenceIndex = 0;

// Interrupts
void IRAM_ATTR coinInserted() {
  CoinAcceptor.pulse_time = millis();
  if (CoinAcceptor.pulse_time - CoinAcceptor.last_pulse_time > 1000) {
    CoinAcceptor.inserted_coin_count++;
    CoinAcceptor.coin_inserted = true;
    CoinAcceptor.last_pulse_time = CoinAcceptor.pulse_time;
  }
}

void IRAM_ATTR buttonPressed(button &pressedButton) {
  pressedButton.pressed_time = millis();
  if (pressedButton.pressed_time - pressedButton.last_pressed_time > 250) {
    pressedButton.pressed = true;
    pressedButton.last_pressed_time = pressedButton.pressed_time;
  }
}

void IRAM_ATTR startButtonPressed() { buttonPressed(StartButton); }
void IRAM_ATTR redButtonPressed() { buttonPressed(RedButton); }
void IRAM_ATTR greenButtonPressed() { buttonPressed(GreenButton); }
void IRAM_ATTR blueButtonPressed() { buttonPressed(BlueButton); }
void IRAM_ATTR yellowButtonPressed() { buttonPressed(YellowButton); }

void setup() {
  // Start serial output for debugging
  Serial.begin(9600);

  for (byte i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(DOOR_LATCH_PIN, OUTPUT);
  pinMode(CoinAcceptor.PIN, INPUT_PULLUP);
  pinMode(StartButton.PIN, INPUT_PULLUP);

  enableAllInterrupts();

  initialiseDisplay();

  delay(1000);
  Serial.printf("Setup completed.\n");
}

void disableAllInterrupts() {
  detachInterrupt(CoinAcceptor.PIN);
  detachInterrupt(StartButton.PIN);
  detachInterrupt(RedButton.PIN);
  detachInterrupt(GreenButton.PIN);
  detachInterrupt(BlueButton.PIN);
  detachInterrupt(YellowButton.PIN);
}

void enableAllInterrupts() {
  attachInterrupt(CoinAcceptor.PIN, coinInserted, RISING);
  attachInterrupt(StartButton.PIN, startButtonPressed, RISING);
  attachInterrupt(RedButton.PIN, redButtonPressed, FALLING);
  attachInterrupt(GreenButton.PIN, greenButtonPressed, FALLING);
  attachInterrupt(BlueButton.PIN, blueButtonPressed, FALLING);
  attachInterrupt(YellowButton.PIN, yellowButtonPressed, FALLING);
}

bool gameCompleted() {
  return (currentLevel() > MAX_GAME_LENGTH);
}

void handleStartButtonPressed() {

  if (gameInProgress()) {
    Serial.printf("Start button pressed but ignored, as game is already running.\n");
    return;
  }

  if (playerHasNoCreditsLeft()) {
    Serial.printf("Insert coin to play the game.\n");
    return;
  }

  startNewGame();
}

void startNewGame() {
  useCredit(1);
  resetGame();
  updateScreen("startingGameCountdownScreen");
  startGame();
}

void resetGame() {
  Serial.printf("Resetting game state.\n");
  resetLevelSequence();
  resetCurrentLevel();
  resetLevelInitialised();
}

void resetLevelSequence() {
  memset(gameSequence, 0, sizeof(gameSequence));
  playerResponseSequenceIndex = 0;
}

void resetStartButton() {
  StartButton.pressed = false;
}

/**
   Lights the given LED and plays a suitable tone
*/
void lightLedAndPlayTone(byte ledIndex) {
  digitalWrite(ledPins[ledIndex], HIGH);
  tone(SPEAKER_PIN, gameTones[ledIndex]);
  delay(300);
  digitalWrite(ledPins[ledIndex], LOW);
  noTone(SPEAKER_PIN);
}

void playSequence() {
  Serial.printf("Playing sequence for level %u.\n", currentLevel());
  for (int i = 0; i <= getGameIndex(); i++) {
    byte currentLed = gameSequence[i];
    lightLedAndPlayTone(currentLed);
    delay(50);
  }
}

void extendColourSequence() {
  randomSeed(analogRead(A0));
  gameSequence[getGameIndex()] = random(0, 4);
}

void gameOver() {
  Serial.printf("Game over! player completed %u level.\n", currentLevel() - 1);
  stopGame();
  delay(200);

  // Play a Wah-Wah-Wah-Wah sound
  tone(SPEAKER_PIN, NOTE_DS5);
  delay(300);
  tone(SPEAKER_PIN, NOTE_D5);
  delay(300);
  tone(SPEAKER_PIN, NOTE_CS5);
  delay(300);
  for (byte i = 0; i < 10; i++) {
    for (int pitch = -10; pitch <= 10; pitch++) {
      tone(SPEAKER_PIN, NOTE_C5 + pitch);
      delay(5);
    }
  }
  noTone(SPEAKER_PIN);
  delay(500);
}

/**
   Plays a hooray sound whenever the user finishes a level
*/
void playLevelUpSound() {
  tone(SPEAKER_PIN, NOTE_E4);
  delay(150);
  tone(SPEAKER_PIN, NOTE_G4);
  delay(150);
  tone(SPEAKER_PIN, NOTE_E5);
  delay(150);
  tone(SPEAKER_PIN, NOTE_C5);
  delay(150);
  tone(SPEAKER_PIN, NOTE_D5);
  delay(150);
  tone(SPEAKER_PIN, NOTE_G5);
  delay(150);
  noTone(SPEAKER_PIN);
}

void resetColourButtons() {
  RedButton.pressed = false;
  GreenButton.pressed = false;
  BlueButton.pressed = false;
  YellowButton.pressed = false;
}

void checkButtonPressWasCorrect(button &colourButton) {
  if (gameSequence[playerResponseSequenceIndex] != colourButton.LED) {
    gameOver();
  }
  
  playerResponseSequenceIndex++;
}

void handleColourButtonPressed(button &colourButton) {
  lightLedAndPlayTone(colourButton.LED);  
  checkButtonPressWasCorrect(colourButton);
  colourButton.pressed = false;
}

bool levelCompleted() {
  return (playerResponseSequenceIndex == currentLevel());
}

bool currentLevelNotInitialised() {
  return (levelInitialised == false);
}

void currentLevelHasBeenInitialised() {
  levelInitialised = true;
}

void resetLevelInitialised() {
  levelInitialised = false;
}

void resetPlayerResponseIndex() {
  playerResponseSequenceIndex = 0;
}

void openPrizeDoorLatch() {
  digitalWrite(DOOR_LATCH_PIN, HIGH);
}

void closePrizeDoorLatch() {
  digitalWrite(DOOR_LATCH_PIN, LOW);
}

void resetCoinAcceptor() {
  CoinAcceptor.coin_inserted = false;
}

void resetAllInterruptFlags() {
  resetStartButton();
  resetColourButtons();
  resetCoinAcceptor();
}

// The solonoid is very noisy and will pull all buttons and the coin acceptor
// low when it is fired.  The voltage drops across the whole board.
// To fix this issue we need to disable all interrupts and 
// then double ensure that all flags are reset.
void unlockPrizeDoor() {
  disableAllInterrupts();

  openPrizeDoorLatch();  
  delay(2000);
  closePrizeDoorLatch();
  delay(500);

  enableAllInterrupts();
  resetAllInterruptFlags();
}

void handlePlayerInputs() {
  if (CoinAcceptor.coin_inserted) {
    resetCoinAcceptor();
    addCredit(1);
    refreshScreen();
    Serial.printf("Coin has been inserted %u times\n", CoinAcceptor.inserted_coin_count);
  }

  if (StartButton.pressed) {
    resetStartButton();
    handleStartButtonPressed();
  }
}

void updateGame() {
  if (gameInProgress()) {

    if (currentLevelNotInitialised()) {
      resetPlayerResponseIndex();
      extendColourSequence();
      delay(1000);
      playSequence();
      resetColourButtons();
      currentLevelHasBeenInitialised();
    }

    if (RedButton.pressed) { handleColourButtonPressed(RedButton); }
    if (GreenButton.pressed) { handleColourButtonPressed(GreenButton); }
    if (BlueButton.pressed) { handleColourButtonPressed(BlueButton); }
    if (YellowButton.pressed) { handleColourButtonPressed(YellowButton); }

    if (levelCompleted()) {
      increaseCurrentLevel();
      resetLevelInitialised();
      refreshScreen();
    }

    if (gameCompleted()) {
      stopGame();
      Serial.println("All levels completed, you have won a prize!");
      delay(2000); // Show some winner text.
      unlockPrizeDoor();
    }
  }
}

void loop() {
  handlePlayerInputs();
  updateGame();
  updateDisplay();
}
