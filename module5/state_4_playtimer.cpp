#include "include/state_4_playtimer.h"

namespace cs334 {

/**
 * @brief Set up the play timer state
 *
 * Save the current time to the play timer state so we have a reference point
 * for how long the game has been going on for, and reset the player member
 * of the game to the correct values based on if it's a seeker or authoritative.
 */
void PlayTimerState::setup() {
  if (m_game->m_player.is_seeker) {
    m_game->m_peripherals_client->setLED(0, 255, 0); // green (seeker)
  } else {
    m_game->m_peripherals_client->setLED(255, 0, 0); // red (hider)
  }
}

/**
 * @brief Run the play timer state
 *
 * Run down a timer for however long we have defined the play period to be. If
 * this ESP is a hider, also continually read in photoresistor input every 0.1s
 * to accumulate in the game's player health member variable.
 */
void PlayTimerState::run() {
  while (true) {
    delay(100);
  }
}

} // namespace cs334