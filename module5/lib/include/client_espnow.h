#ifndef CLIENT_ESPNOW_H
#define CLIENT_ESPNOW_H

#include <stdbool.h>
#include "freeRTOS/task.h"
#include "config.h"

using namespace cs334;

class ESPNOWClient {
public: // METHODS
  ESPNOWClient();

  /**
   * @brief Begins an RTOS task, scanning for ESP-NOW connections
   * This is used to initialize the m_connected_players array.
   */
  void begin_scan();

  /**
   * @brief xTask used to continually update m_connected_players with new connections
   */
  void scan_task(void * pvParameter);

  /**
   * @brief Ends the RTOS task scanning for ESP-NOW connections
   * This allows us to know when our m_connected_players object is ready.
   */
  void end_scan();

  /**
   * @brief Sends an ESP-NOW message to a specific MAC address.
   * 
   * Note that ESP-NOW messages are limited in size, to something like 256 bytes.
   * So we should make sure to keep our messages super short.
   * 
   * @param mac_address 
   */
  void send(const char* message, uint8_t* mac_address);

  /**
   * @brief Broadcasts an ESP-NOW message to all connected players.
   * 
   * This function will be used by the authoritative node 
   * 
   * @param mac_address 
   */
  void send(const char* message);

public: // MEMBERS 

  // maps MAC addresses to player states. use this as a "set" for 
  std::map<std::string, player_state_t> m_connected_players;

private: // MEMBERS
  TaskHandle_t m_scan_task_handle = NULL;
}

#endif /* CLIENT_ESPNOW_H */
