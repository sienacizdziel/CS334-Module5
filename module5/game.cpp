#include "include/game.h"

// include all our states.
#include <string>

#include "include/state_1_connection.h"
#include "include/state_2_initialization.h"
#include "include/state_3_hidetimer.h"
#include "include/state_4_playtimer.h"
#include "include/state_5_ranking.h"

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
  // build our ESP and Peripherals clients, initializing both
  Client::ESPNOW::setup(&m_players, m_player.is_authoritative);
  m_peripherals_client = new Client::Peripherals();

  // build our base player object
  // get our nodeID dynamically (UNTESTED)
  m_player.node_id = Client::ESPNOW::getNodeId();

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
  for (GameState *state : m_states)
    delete state;
  m_states.clear();
  delete m_peripherals_client;
}

/**
 * @brief Synchronously iterate through each state
 *
 */
void Game::loop() {
  Serial.println("Game started!");
  for (const auto &state : m_states) {
    state->setup();
    state->run();
  }
}

};  // namespace cs334