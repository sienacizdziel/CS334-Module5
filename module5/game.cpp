#include "include/game.h"

// include all our states.
#include <string>
#include "include/states/state_connection.h"
#include "include/states/state_initialization.h"
#include "include/states/state_hidetimer.h"
#include "include/states/state_playtimer.h"
#include "include/states/state_ranking.h"

#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

namespace cs334 {

/**
 * @brief Construct a new Game::Game object
 * 
 * Initializes all the state objects
 */
Game::Game() {
  // build our base player object
  // get our mac address dynamically (UNTESTED)
  m_player.mac_address = std::string(WiFi.macAddress().c_str());
  // build our ESP and Peripherals clients
  m_esp_client = new Client::ESPNOW(m_player.mac_address);
  m_peripherals_client = new Client::Peripherals();
  // add all of the states in order to the m_states vector
  m_states.push_back(new ConnectionState(this));
  m_states.push_back(new InitializationState(this));
  m_states.push_back(new HideTimerState(this));
  m_states.push_back(new PlayTimerState(this));
  m_states.push_back(new RankingState(this));
}

/**
 * @brief Destroy the Game:: Game object
 */
Game::~Game() {
  // delete all the states we allocated
  for (GameState* state : m_states) delete state;
  m_states.clear();
  delete m_esp_client;
  delete m_peripherals_client;
}

/**
 * @brief Synchronously iterate through each state
 * 
 */
void Game::loop() {
  for (const auto& state : m_states) {
    state->setup();
    state->run();
  }
}

};