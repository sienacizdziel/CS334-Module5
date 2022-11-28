#ifndef STATE_HIDETIMER_H
#define STATE_HIDETIMER_H

#include "game.h"
#include "state_interface.h"
#include <chrono>
#include <ctime>
#include <iostream>

namespace cs334 {

class HideTimerState : public GameState {
public:
  HideTimerState(Game *game) : GameState("hide_timer", game) {}
  void setup() override;
  void run() override;

private:
  // time at which the hide timer state began
  std::chrono::time_point<std::chrono::system_clock> start;
};

}; // namespace cs334

#endif /* STATE_HIDETIMER_H */
