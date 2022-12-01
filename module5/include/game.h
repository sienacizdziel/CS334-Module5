#ifndef GAME_H
#define GAME_H

#include <vector>

#include "client_espnow.h"
#include "client_peripherals.h"
#include "config.h"
#include "state_interface.h"

namespace cs334 {

class Game {
 public:
  Game();       // builds the game instance with default values.
  ~Game();      // destructor frees all of the allocated states
  void loop();  // iterates through all states a single time.

 public:
  // the player running this instance of the game
  player_state_t m_player{
      .is_seeker = false, .is_authoritative = true, .health = 0};

  // ESP-NOW client for connecting to other players
  // Peripherals client for receiving / sending peripheral states
  Client::Peripherals *m_peripherals_client;
  int num_initial_players = 0;

 private:
  // states of the game to iterate through
  std::vector<GameState *> m_states;
};

};  // namespace cs334

#endif /* GAME_H */
