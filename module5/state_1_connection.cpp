#include "include/state_1_connection.h"

#include <Arduino.h>

namespace cs334 {

/**
 * @brief Set up the connection state
 *
 * Should call the ESPNOW::beginScan function to start the task for listening
 * for new connections. Most of the work for this state is actually work done
 * by the ESP-NOW client. Set the LED here to blink blue here as well.
 */
void ConnectionState::setup() {
  Serial.println("connection state setup");
  m_game->m_peripherals_client->setLED(0, 0, 255);              // blinking blue
  m_game->m_peripherals_client->m_button_press_duration = 0.f;  // reset button duration listener
}

/**
 * @brief Run the connection state
 *
 * Listen for a 5-second button press, at which point you should break out of
 * the loop, indicating a transition to the "initialization" state.
 */
void ConnectionState::run() {
  while (m_game->m_peripherals_client->m_button_press_duration < 5.f) {
    Serial.printf("Button press: %.2f\n", m_game->m_peripherals_client->m_button_press_duration);
    delay(100);
  }
}

}  // namespace cs334