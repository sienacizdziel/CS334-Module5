#include "game.h"

// include all our states.
#include "states/state_connection.h"
#include "states/state_initialization.h"
#include "states/state_hidetimer.h"
#include "states/state_playtimer.h"
#include "states/state_ranking.h"

#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

using namespace cs334;

/**
 * @brief Construct a new Game::Game object
 * 
 * Initializes all the state objects
 */
Game::Game() {
  // build our base player object
  m_player = {
    .is_seeker = false,
    .is_authoritative = false,
    .health = 0,
    // get our mac address dynamically (UNTESTED)
    .mac_address = WiFi.macAddress() 
  };
  // build our ESP client
  m_esp_client = ESPNOWClient(m_player.mac_address);
  // add all of the states in order to the m_states vector
  m_states.push_back(ConnectionState());
  m_states.push_back(InitializationState());
  m_states.push_back(HideTimerState());
  m_states.push_back(PlayTimerState());
  m_states.push_back(RankingState());
}

/**
 * @brief Synchronously iterate through each state
 * 
 */
void Game::loop() {
  for (const auto& state : m_states) {
    state.setup();
    state.run();
  }
}