#ifndef STATE_1_CONNECTION_H
#define STATE_1_CONNECTION_H

#include "game.h"
#include "state_interface.h"

namespace cs334 {

class ConnectionState : public GameState {
public:
  ConnectionState(Game *game) : GameState("connection", game) {}
  void setup() override;
  void run() override;

private:
};

}; // namespace cs334

#endif /* STATE_1_CONNECTION_H */
