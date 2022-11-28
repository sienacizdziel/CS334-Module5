#ifndef STATE_2_INITIALIZATION_H
#define STATE_2_INITIALIZATION_H

#include "game.h"
#include "state_interface.h"

namespace cs334 {

class InitializationState : public GameState {
public:
  InitializationState(Game *game) : GameState("initialization", game) {}
  void setup() override;
  void run() override;

private:
};

}; // namespace cs334

#endif /* STATE_2_INITIALIZATION_H */
