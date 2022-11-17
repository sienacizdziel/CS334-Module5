#ifndef STATE_PLAYTIMER_H
#define STATE_PLAYTIMER_H

#include "states/state_interface.h"

namespace cs334 {

class PlayTimerState: GameState {
public:
  PlayTimerState(player_state_t* player_state) : GameState("play_timer", player_state) {}
  void setup() override;
  void run() override;
private:
  
};

};


#endif /* STATE_PLAYTIMER_H */
