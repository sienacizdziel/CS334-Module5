#ifndef CLIENT_ESPNOW_H
#define CLIENT_ESPNOW_H

#include <stdbool.h>
#include <map>
#include <string>

#include "freeRTOS/FreeRTOS.h"
#include "freeRTOS/task.h"
#include "config.h"

namespace cs334::Client {

class ESPNOW {
public: // METHODS
  ESPNOW(std::string mac_address);
  ~ESPNOW();

  /**
   * @brief Begins an RTOS task, scanning for ESP-NOW connections
   * This is used to initialize the m_connected_players array.
   */
  void beginScan();

  /**
   * @brief Ends the RTOS task scanning for ESP-NOW connections
   * This allows us to know when our m_connected_players object is ready.
   */
  void endScan();

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

  /**
   * @brief xTask used to continually update m_connected_players with new connections
   * 
   * We use static so we can initiate this into its own xTaskCreate function.
   */
  void _scanTask(void);

public: // MEMBERS 

  // save our mac address to be able to sort all MAC addresses
  std::string m_mac_address;
  // maps MAC addresses to player states
  std::map<std::string, player_state_t> m_connected_players;

private: // MEMBERS
  TaskHandle_t m_scan_task_handle = NULL;

  /**
   * @brief Static wrapper around scan_task, used for starting the FreeRTOS task
   */
  static void _scanTaskImpl(void * _this);
};

};

#endif /* CLIENT_ESPNOW_H */
