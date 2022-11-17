#ifndef STATE_HIDETIMER_H
#define STATE_HIDETIMER_H

#include "states/state_interface.h"

namespace cs334 {

class HideTimerState: GameState {
public:
  HideTimerState(player_state_t* player_state) : GameState("hide_timer", player_state) {}
  void setup() override;
  void run() override;
private:
  
};

};


#endif /* STATE_HIDETIMER_H */
