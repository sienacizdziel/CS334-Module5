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
    m_game->m_peripherals_client->setLED(255, 0, 0);  // red (seeker)
  } else {
    m_game->m_peripherals_client->setLED(0, 255, 0);  // green (hider)
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
  // delay for the hide timer MS
  auto start = std::chrono::system_clock::now();
  while (true) {
    // update the peripherals (LEDs mostly)
    m_game->m_peripherals_client->update();
    // update photoresistor health
    m_game->m_player.health += m_game->m_peripherals_client->getPhotoresistorInput();
    // get the current system clock time
    auto curr = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = curr - start;
    // if the timer was exceeded, move to the next state
    if (elapsed_seconds.count() >= TIME_PLAYING_SECONDS)
      break;
    // check every 0.1s
    delay(100);
  }
  // Once done, print out the health
  Serial.printf("[STATUS] Finished play state with health: %u\n", m_game->m_player.health);
}

}  // namespace cs334