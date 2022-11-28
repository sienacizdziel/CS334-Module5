#include "include/state_5_ranking.h"

namespace cs334 {

/**
 * @brief Set up the ranking state
 *
 *
 */
void RankingState::setup() {
  m_game->m_peripherals_client->setLED(255, 0, 255); // purple
}

/**
 * @brief Run the connection state
 *
 * Wait for messages from all peers in the game indicating their player states,
 * i.e. were they seekers or hiders, what was their health, etc. Then,
 * listen for a 5-second button press, at which point you should break out of
 * the loop, indicating a transition to the "connection" state.
 */
void RankingState::run() {
  while (true) {
    delay(100);
  }
}

} // namespace cs334