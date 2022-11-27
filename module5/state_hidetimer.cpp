#include "include/states/state_hidetimer.h"

// UNTESTED SINCE I DON'T HAVE AN ESP32 AND ALSO DON'T KNOW C++ :(

namespace cs334 {

/**
 * @brief Set up the hide timer state
 * 
 * 
 */
void HideTimerState::setup() {
  start = std::chrono::system_clock::now();
  Peripherals::setLED(255, 255, 255);
}

/**
 * @brief Run the hide timer state
 * 
 * Loop and compare the current time to the time at which `setup` was run. Once
 * that time exceeds the total timer's time, break out of the loop and continue on
 * to the next state, the play timer state.
 */
void HideTimerState::run() {
  while (true) {
    auto curr = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = curr - start;
    std::cout << "started hiding at " << start
              << "elapsed time: " << elapsed_seconds.count() << "s"
              << "current time is " << curr
              << std::endl;
    if (elapsed_seconds.count() >= HIDE_TIME) {
      break;
    }
    delay(100);
  }
}

}