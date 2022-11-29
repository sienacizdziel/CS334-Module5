#include "include/game.h"

cs334::Game *game;

void setup() {
  Serial.begin(115200);
  game = new cs334::Game();
}

void loop() {
  game->loop();
}
