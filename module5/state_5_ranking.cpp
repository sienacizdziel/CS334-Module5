#include "include/state_5_ranking.h"

#include <map>

#include "include/config.h"

namespace cs334 {

/**
 * @brief Set up the ranking state
 */
void RankingState::setup() {
  Serial.println("[STATE] Began (5) Ranking.");
  Client::ESPNOW::beginScan();
  Client::ESPNOW::setAcceptingNewConnections(true);
  // if authoritative
  if (m_game->m_player.is_authoritative) {
    // set the color to a solid purple
    m_game->m_peripherals_client->setLED(255, 0, 255);  // purple
  } else {
    // other players turn to a flashing purple
    m_game->m_peripherals_client->setLED(255, 0, 255, 500);  // flashing purple
  }
  // set broadcast to finalized health. if we were the seeker, make the health
  // the maximum possibel value (effectively removing us from the ranking)
  uint32_t health = m_game->m_player.is_seeker ? INFINITY : m_game->m_player.health;
  Client::ESPNOW::sendBroadcast(Client::ESPNOWEvent::EventType::HEALTH, health);
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
  // if authoritative
  if (m_game->m_player.is_authoritative) {
    //  loop until have received all players' health values
    uint32_t winner;
    uint32_t winner_health = INFINITY;
    int n_players_with_health = 0;
    do {
      m_game->m_peripherals_client->update();
      n_players_with_health = 0;
      std::map<uint32_t, player_state_t>::iterator it;
      for (it = m_game->m_players.begin(); it != m_game->m_players.end(); it++) {
        if (it->second.health > 0) n_players_with_health++;
        // note we do the ranking logic here as well
        if (it->second.health < winner_health) {
          winner = it->first;
          winner_health = it->second.health;
        }
      }
    } while (n_players_with_health != m_game->m_players.size());
    // check if ourself is winner
    if (m_game->m_player.health < winner_health) {
      winner = Client::ESPNOW::getNodeId();
      winner_health = m_game->m_player.health;
      m_game->m_player.is_winner = true;
    }
    Serial.printf("[AUTHORITATIVE] Winner: %u at %u health\n", winner, winner_health);
    // finally, broadcast the winner to all other ESPs
    Client::ESPNOW::sendBroadcast(Client::ESPNOWEvent::EventType::RANK, winner);
  } else {  // if non-authoritative
    // loop until rank has been assigned
    while (!Client::ESPNOW::hasRank()) {
      m_game->m_peripherals_client->update();
    }
  }
  // once ready to move on, turn off accepting of new connections
  Client::ESPNOW::setAcceptingNewConnections(false);
  // now if winner, flash yellow quickly until button press
  if (m_game->m_player.is_winner)
    m_game->m_peripherals_client->setLED(255, 255, 0, 250);
  // if loser, just hold low-intensity steady yellow
  else
    m_game->m_peripherals_client->setLED(127, 127, 0);
  Serial.println("[GAME] Finished.");
  while (!m_game->m_peripherals_client->checkButtonPressDuration(5000)) {
    m_game->m_peripherals_client->update();
  }
}

}  // namespace cs334
