#include "include/state_4_playtimer.h"

#include <chrono>

namespace cs334 {

/**
 * @brief Set up the play timer state
 *
 * Save the current time to the play timer state so we have a reference point
 * for how long the game has been going on for, and reset the player member
 * of the game to the correct values based on if it's a seeker or authoritative.
 */
void PlayTimerState::setup() {
  Serial.println("[STATE] Began (4) Play Timer.");
  if (m_game->m_player.is_seeker) {
    m_game->m_peripherals_client->setLED(0, 255, 0);  // green (seeker)
  } else {
    m_game->m_peripherals_client->setLED(255, 0, 0);  // red (hider)
  }
  m_game->m_player.health = 0;
  start = millis();
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
    // add photoresistor damage to player's accumulated health
    // TODO potentially add a multiplier for higher values (direct flashlight contact)
    m_game->m_player.health += m_game->m_peripherals_client->getPhotoresistorInput();

    // TODO possibly update LED flashing rate or brightness to indicate loss in health

    // if we've been playing for the total match time, move on to the next state
    if ((millis() - start) > (TIME_PLAYING_SECONDS * 1000)) {
      break;
    }

    // adjust the delay to speed up or slow down damage
    delay(100);
  }
}

}  // namespace cs334