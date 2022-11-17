#ifndef GAME_H
#define GAME_H

#include "config.h"

using namespace cs334;

/**
 * @brief An abstract base class for streamlining game states
 * 
 * Stores a reference to the player state, which is the common element which
 * must be updated throughout different game states.
 */
class GameState {
public:
  GameState(const char* name, player_state_t* player_state) {
    m_name = name;
    m_player = player_state;
  }

  /**
   * @brief A setup function for the state
   * 
   * Use this to make sure each state resets values that it will be changing
   * to defaults. For example, the connection state should reset the ESPNOWClient
   * peer list as well as the authoritative node, and the play state should
   * reset the health, and the hide timer should reset the timer, etc.
   */
  virtual void setup();

  /**
   * @brief A run function for executing the state
   * When inheriting GameState, override this for specialized functionality.
   */
  virtual void run();

public:
  const char* m_name;
  player_state_t* m_player;
}

#endif /* GAME_H */
