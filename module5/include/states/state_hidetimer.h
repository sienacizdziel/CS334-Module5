#ifndef STATE_HIDETIMER_H
#define STATE_HIDETIMER_H

#include "state_interface.h"

namespace cs334 {

class Game;

class HideTimerState: public GameState {
public:
  HideTimerState(Game* game) : GameState("hide_timer", game) {}
  void setup() override;
  void run() override;
private:
  
};

};


#endif /* STATE_HIDETIMER_H */
