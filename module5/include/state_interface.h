#ifndef STATE_INTERFACE_H
#define STATE_INTERFACE_H

#include "config.h"

namespace cs334 {

class Game;

/**
 * @brief An abstract base class for streamlining game states
 *
 * Stores a reference to the player state, which is the common element which
 * must be updated throughout different game states.
 */
class GameState {
public:
  GameState(const char *name, Game *game) {
    m_name = name;
    m_game = game;
  }
  virtual ~GameState() = default;

  /**
   * @brief A setup function for the state
   *
   * Use this to make sure each state resets values that it will be changing
   * to defaults. For example, the connection state should reset the
   * ESPNOWClient peer list as well as the authoritative node, and the play
   * state should reset the health, and the hide timer should reset the timer,
   * etc.
   */
  virtual void setup();

  /**
   * @brief A run function for executing the state
   * When inheriting GameState, override this for specialized functionality.
   */
  virtual void run();

protected:
  const char *m_name;
  Game *m_game;
};

}; // namespace cs334

#endif /* STATE_INTERFACE_H */
