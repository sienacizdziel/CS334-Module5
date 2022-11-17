#ifndef STATE_CONNECTION_H
#define STATE_CONNECTION_H

#include "states/state_interface.h"

namespace cs334 {

class ConnectionState: GameState {
public:
  ConnectionState(player_state_t* player_state) : GameState("connection", player_state) {}
  void setup() override;
  void run() override;
private:
  
};

};

#endif /* STATE_CONNECTION_H */
