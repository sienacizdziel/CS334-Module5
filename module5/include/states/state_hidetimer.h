#ifndef STATE_HIDETIMER_H
#define STATE_HIDETIMER_H

#include "state_interface.h"
#include <iostream>
#include <chrono>
#include <ctime>

#define HIDE_TIME 300 // hide time set as 5 min

namespace cs334 {

class Game;

class HideTimerState: public GameState {
public:
  HideTimerState(Game* game) : GameState("hide_timer", game) {}
  void setup() override;
  void run() override;
private:
  auto start;
};

};


#endif /* STATE_HIDETIMER_H */
