// Game logic
// Author: @DeveloperDIY
//
byte gameIndex = 0;
bool game_running = false;

byte getGameIndex() {
  return gameIndex;
}

bool firstLevel() {
  return (gameIndex == 0); 
}

void resetCurrentLevel() {
  gameIndex = 0;
}

void increaseCurrentLevel() {
  gameIndex++;
}

extern byte currentLevel() {
  return gameIndex + 1;
}

void startGame() {
  Serial.printf("Starting a new game.\n");
  game_running = true;
}

void stopGame() {
  game_running = false;
}

bool gameInProgress() {
  return (game_running == true);
}

bool gameNotStarted() {
  return (game_running == false);
}
