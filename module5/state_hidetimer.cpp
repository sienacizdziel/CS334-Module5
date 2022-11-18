#include "include/states/state_hidetimer.h"

namespace cs334 {

/**
 * @brief Set up the hide timer state
 * 
 * 
 */
void HideTimerState::setup() {

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
    delay(100);
  }
}

}