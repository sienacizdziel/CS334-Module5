#ifndef STATE_CONNECTION_H
#define STATE_CONNECTION_H

#include "state_interface.h"

namespace cs334 {

class Game;

class ConnectionState: public GameState {
public:
  ConnectionState(Game* game) : GameState("connection", game) {}
  void setup() override;
  void run() override;
private:
  
};

};

#endif /* STATE_CONNECTION_H */
