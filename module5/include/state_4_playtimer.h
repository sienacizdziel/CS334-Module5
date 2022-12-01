#ifndef STATE_4_PLAYTIMER_H
#define STATE_4_PLAYTIMER_H

#include "game.h"
#include "state_interface.h"

namespace cs334 {

class PlayTimerState : public GameState {
 public:
  PlayTimerState(Game *game) : GameState("play_timer", game) {}
  void setup() override;
  void run() override;

 private:
  unsigned long start;
};

};  // namespace cs334

#endif /* STATE_4_PLAYTIMER_H */
