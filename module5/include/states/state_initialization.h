#ifndef STATE_INITIALIZATION_H
#define STATE_INITIALIZATION_H

#include "state_interface.h"

namespace cs334 {

class Game;

class InitializationState: public GameState {
public:
  InitializationState(Game* game) : GameState("initialization", game) {}
  void setup() override;
  void run() override;
private:
  
};

};

#endif /* STATE_INITIALIZATION_H */
