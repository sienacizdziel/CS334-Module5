#ifndef STATE_PLAYTIMER_H
#define STATE_PLAYTIMER_H

#include "state_interface.h"

namespace cs334 {

class Game;

class PlayTimerState: public GameState {
public:
  PlayTimerState(Game* game) : GameState("play_timer", game) {}
  void setup() override;
  void run() override;
private:
  
};

};


#endif /* STATE_PLAYTIMER_H */
