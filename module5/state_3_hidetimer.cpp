#include "include/state_3_hidetimer.h"

#include "include/config.h"

// UNTESTED SINCE I DON'T HAVE AN ESP32 AND ALSO DON'T KNOW C++ :(

namespace cs334 {

/**
 * @brief Set up the hide timer state
 *
 * Set the LED state to white, and save a reference to the current time.
 */
void HideTimerState::setup() {
  Serial.println("[STATE] Began (3) Hide Timer.");
  m_game->m_peripherals_client->setLED(255, 255, 255);  // white
}

/**
 * @brief Run the hide timer state
 *
 * Loop and compare the current time to the time at which `setup` was run. Once
 * that time exceeds the total timer's time, break out of the loop and continue
 * on to the next state, the play timer state.
 */
void HideTimerState::run() {
  // delay for the hide timer MS
  auto start = std::chrono::system_clock::now();
  while (true) {
    // if the timer was exceeded, move to the next state
    if ((std::chrono::system_clock::now() - start).count() >= TIME_HIDING_SECONDS)
      break;
    // check every 0.1s
    delay(100);
  }
}

}  // namespace cs334