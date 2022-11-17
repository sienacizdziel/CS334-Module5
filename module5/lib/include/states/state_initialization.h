#ifndef STATE_INITIALIZATION_H
#define STATE_INITIALIZATION_H

#include "states/state_interface.h"

using namespace cs334;

class InitializationState: GameState {
public:
  InitializationState(player_state_t* player_state) : GameState("initialization", player_state) {}
  void setup() override;
  void run() override;
private:
  
}

#endif /* STATE_INITIALIZATION_H */
