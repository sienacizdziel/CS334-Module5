#include "lib/include/game.h"

cs334::Game *game;

void setup() {
  game = new cs334::Game();
}

void loop() {
  game->loop();
}
