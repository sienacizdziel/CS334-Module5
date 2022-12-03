#ifndef STATE_HIDETIMER_H
#define STATE_HIDETIMER_H

#include <chrono>
#include <ctime>
#include <iostream>

#include "game.h"
#include "state_interface.h"

namespace cs334 {

class HideTimerState : public GameState {
 public:
  HideTimerState(Game *game) : GameState("hide_timer", game) {}
  void setup() override;
  void run() override;
};

};  // namespace cs334

#endif /* STATE_HIDETIMER_H */
