#include "include/state_5_ranking.h"

#include <map>

#include "include/config.h"

namespace cs334 {

/**
 * @brief Set up the ranking state
 *
 *
 */
void RankingState::setup() {
  Serial.println("[STATE] Began (5) Ranking.");
  m_game->m_peripherals_client->setLED(255, 0, 255);  // purple
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
  if (m_game->m_player.is_authoritative) {
    // wait for everyone to reconnect
    // while (m_authoritative_connected_nodes.size < m_esp_client->m_connected_players.size) {};
    // then receive all the scores in a map
    std::map<std::string, u_int16_t> healths;
    // healths.sort();
    // broadcast message to lowest health mac_address with WINNER
    // wait for button press to continue to next state
  } else {
    // normal node
    for (;;) {
      // broadcast message to authoritative node with health
      // listen for WINNER message
      // listen for RESET message
      // if RESET message received -> break;
    }
  }
}

}  // namespace cs334
