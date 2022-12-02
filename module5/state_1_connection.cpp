#include "include/state_1_connection.h"

#include <Arduino.h>

#include "painlessMesh.h"

namespace cs334 {

/**
 * @brief Set up the connection state
 *
 * Should call the ESPNOW::beginScan function to start the task for listening
 * for new connections. Most of the work for this state is actually work done
 * by the ESP-NOW client. Set the LED here to blink blue here as well.
 */
void ConnectionState::setup() {
  Serial.printf("STATE: Connection");
  m_game->m_peripherals_client->setLED(0, 0, 255);  // blinking blue
  m_game->m_players.clear();
  Client::ESPNOW::beginScan();
  Client::ESPNOW::setAcceptingNewConnections(true);
}

/**
 * @brief Run the connection state
 *
 * Listen for a 5-second button press, at which point you should break out of
 * the loop, indicating a transition to the "initialization" state.
 */
void ConnectionState::run() {
  while (m_game->m_peripherals_client->m_button_press_duration < 5.f) {
    delay(10);
  }
  // turn off accepting of new connections
  Client::ESPNOW::setAcceptingNewConnections(false);
}

}  // namespace cs334