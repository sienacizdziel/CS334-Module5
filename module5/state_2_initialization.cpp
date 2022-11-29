#include "include/state_2_initialization.h"

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
  Serial.println("initialization state setup");
  // start calibrating the photoresistors
  m_game->m_peripherals_client->beginPhotoresistorCalibration();
  // set the initialization LED color
  if (m_game->m_player.is_authoritative) {
    m_game->m_peripherals_client->setLED(255, 255, 0); // yellow
  } else {
    m_game->m_peripherals_client->setLED(0, 0, 255); // blue
  }
}

/**
 * @brief Run the initialization state
 *
 * Listen for a 5-second button press, at which point you should break out of
 * the loop, indicating a transition to the "hide timer" state.
 */
void InitializationState::run() {
  while (true) {
    delay(100);
  }
}

} // namespace cs334