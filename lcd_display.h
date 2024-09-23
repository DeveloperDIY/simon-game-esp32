// TFT LCD display logic
// Author: @DeveloperDIY
// 
// Configuration for tft_espi user_setup.h
// #define ILI9341_DRIVER  
// #define TFT_RGB_ORDER TFT_BGR
// #define TFT_MOSI 11 // SDA
// #define TFT_SCLK 12 // SCL
// #define TFT_CS   10 // CS
// #define TFT_DC    9 // DC
// #define TFT_RST  -1 // REs
// #define SPI_FREQUENCY  40000000
// #define USE_HSPI_PORT

#include <TFT_eSPI.h>
#include <SPI.h>

// Load fonts
#include "GilroyMedium24.h"
#include "GilroyHeavy64.h"
#include "GilroyHeavy96.h"
#define FONT_GILROY_MEDIUM_24 GilroyMedium24
#define FONT_GILROY_HEAVY_64 GilroyHeavy64
#define FONT_GILROY_HEAVY_96 GilroyHeavy96

int screenWidth = 240;
int screenHeight = 320;
extern String currentScreen = "";
extern TFT_eSPI tft = TFT_eSPI();

void initialiseDisplay() {
  tft.init();
  tft.setRotation(2);
}

void clearScreen() {
  tft.fillScreen(TFT_BLACK);
}

void refreshScreen() {
  currentScreen = "refresh";
}

String creditTextLine() {
  char creditStr[14];
  String creditWord = "CREDIT";
  if (getCredits() > 1) creditWord = "CREDITS";

  sprintf(creditStr, "%u CREDITS", getCredits());

  return creditStr;
}

TFT_eSprite createCreditsSprite() {
  TFT_eSprite creditsSprite = TFT_eSprite(&tft);  // Declare Sprite object "creditsSprite" with pointer to "tft" object
  creditsSprite.createSprite(200, 30);
  creditsSprite.fillSprite(TFT_BLACK);
  creditsSprite.setTextDatum(MC_DATUM);
  creditsSprite.setTextColor(TFT_YELLOW, TFT_BLACK);
  creditsSprite.loadFont(FONT_GILROY_MEDIUM_24);

  creditsSprite.drawString(creditTextLine(), 100, 16);

  return creditsSprite;
}

/**
 * When the game is not running and the player has no credits.
 * Ask the user to insert a coin to play.
 */
void insertCoinScreen() {
  clearScreen();

  tft.setTextDatum(MC_DATUM); // Center text
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);

  tft.loadFont(FONT_GILROY_HEAVY_64);
  tft.drawString("INSERT", 120, 90);
  tft.drawString("COIN", 120, 150);
  tft.unloadFont();

  tft.loadFont(FONT_GILROY_MEDIUM_24);
  tft.drawString("TO PLAY", 120, 250);
  tft.unloadFont();
}

void startGameScreen() {
  clearScreen();

  // Setup screen
  tft.setTextDatum(MC_DATUM); // Center text
  tft.setTextColor(TFT_GREEN, TFT_BLACK);

  // START GAME
  tft.loadFont(FONT_GILROY_HEAVY_64);
  tft.drawString("START", 120, 90);
  tft.drawString("GAME", 120, 150);
  tft.unloadFont();

  // CREDITS
  createCreditsSprite().pushSprite(20, 250);
}

void startingGameCountdownScreen() {
  clearScreen();

  tft.setTextDatum(MC_DATUM); // Center text
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);

  tft.loadFont(FONT_GILROY_HEAVY_96);
  tft.drawString("5", 120, 170);
  tft.unloadFont();

  int size = sizeof(pirates_durations) / sizeof(int);

  for (int note = 0; note < size; note++) {
    runningTime = millis();
    //to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int duration = 1000 / pirates_durations[note];
    tone(SPEAKER_PIN, pirates_notes[note], duration);

    //to distinguish the notes, set a minimum time between them.
    //the note's duration + 30% seems to work well:
    int pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);

    //stop the tone playing:
    noTone(SPEAKER_PIN);
  }
}

void gameRunningScreen() {
  clearScreen();

  tft.setTextDatum(MC_DATUM); // Center text
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);

  tft.loadFont(FONT_GILROY_HEAVY_64);
  tft.drawString("LEVEL", 120, 80);
  tft.unloadFont();
  
  tft.loadFont(FONT_GILROY_HEAVY_96);
  tft.drawString(String(currentLevel()), 120, 170);
  tft.unloadFont();

  createCreditsSprite().pushSprite(20, 270);
}


void updateScreen(String screen) {
  currentScreen = screen;

  if (screen == "insertCoinScreen") insertCoinScreen();
  if (screen == "startGameScreen") startGameScreen();
  if (screen == "startingGameCountdownScreen") startingGameCountdownScreen();
  if (screen == "gameRunningScreen") gameRunningScreen();
}

void updateDisplay() {
   String screen = currentScreen;

  if (gameNotStarted() && playerHasNoCreditsLeft()) {
    screen = "insertCoinScreen";
  }
  if (gameNotStarted() && playerHasCredits()) {
    screen = "startGameScreen";
  }
  if (gameInProgress()) {
    screen = "gameRunningScreen";
  }

  if (currentScreen != screen) {
    updateScreen(screen);
  }
}


