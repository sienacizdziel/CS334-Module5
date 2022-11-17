#ifndef GAME_H
#define GAME_H

#include "states/state_interface.h"
#include <vector>

using namespace cs334;


class Game {
public:
  Game(); // builds the game instance with default values.
  void loop(); // iterates through all states a single time.
private:
  // the player running this instance of the game
  player_state_t m_player;

  // ESP-NOW client for connecting to other players
  ESPNOWClient m_esp_client;

  // states of the game to iterate through
  std::vector<GameState> m_states;
}

#endif /* GAME_H */
