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
    m_game->m_peripherals_client->setLED(0, 255, 0);  // green (seeker)
    m_game->m_player.health = INITIAL_HEALTH;
  } else {
    m_game->m_peripherals_client->setLED(255, 0, 0);  // red (hider)
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
  float photoresistor_value;

  while (true) {
    photoresistor_value = m_game->m_peripherals_client->getPhotoresistorInput();
    if (photoresistor_value > PHOTORESISTOR_CUTOFF_HIGH) {
      m_game->m_player.health -= HIGH_DAMAGE;
    } else if (photoresistor_value > PHOTORESISTOR_CUTOFF_LOW) {
      m_game->m_player.health -= LOW_DAMAGE;
    }

    // TODO update LED flashing rate or brightness to indicate loss in health

    if (m_game->m_player.health <= 0) {
      // TODO indicate death via LED
      break;
    }
    // adjust the delay to speed up or slow down damage
    delay(500);
  }
}

}  // namespace cs334