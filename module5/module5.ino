#include "include/game.h"

cs334::Game *game;

void setup() {
  Serial.begin(115200);
  Serial.println("Beginning game!");
  game = new cs334::Game();
}

void loop() {
  Serial.println("Looping game!");
  game->loop();
}
