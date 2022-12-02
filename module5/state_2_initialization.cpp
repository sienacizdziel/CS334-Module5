#include "include/state_2_initialization.h"

#include <Arduino.h>

namespace cs334 {

/**
 * @brief Set up the initialization state
 *
 * Determines who is the authority node in the network by alphabetically sorting
 * the MAC addresses of all nodes in the network (including this one). If the
 * authority is determined to be this node, then we need to initialize all other
 * nodes. If not, then we need to listen for ESP-NOW messages containing
 * initialization information (i.e. whether or not one is the seeker)
 */
void InitializationState::setup() {
  Serial.printf("STATE: Initialization");
  // start calibrating the photoresistors
  m_game->m_peripherals_client->beginPhotoresistorCalibration();

  // get the list of players in the game, which is set now
  long seeker_i = random(0, m_game->m_players.size() + 1);
  uint32_t seeker_id;
  // if we picked ourselves, we are the seeker
  if (seeker_i == m_game->m_players.size()) {
    seeker_id = Client::ESPNOW::getNodeId();
    m_game->m_player.is_seeker = true;
    // otherwise, randomly select our seeker based on seeker_i
  } else {
    auto iter_players = m_game->m_players.begin();
    std::advance(iter_players, seeker_i);
    seeker_id = iter_players->first;
  }
  // now broadcast who the seeker is to all nodes in the mesh so they can know
  Client::ESPNOW::sendBroadcast(Client::ESPNOWEvent::EventType::ASSIGN, seeker_id);

  // set the initialization LED color
  if (m_game->m_player.is_authoritative) {
    m_game->m_peripherals_client->setLED(255, 255, 0);  // yellow
  } else {
    m_game->m_peripherals_client->setLED(0, 0, 255);  // blue
  }
}

/**
 * @brief Run the initialization state
 *
 * Listen for a 5-second button press, at which point you should break out of
 * the loop, indicating a transition to the "hide timer" state.
 */
void InitializationState::run() {
  while (m_game->m_peripherals_client->m_button_press_duration < 5.f) {
    delay(10);
  }
  Client::ESPNOW::endScan();
}

}  // namespace cs334