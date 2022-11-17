#include "lib/include/game.h"

cs334::Game *game;

void setup() {
  game = new Game();
}

void loop() {
  game.loop();
}
