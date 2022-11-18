#include "include/client_espnow.h"
#include <functional>

namespace cs334::Client {

/**
 * @brief Construct a new ESPNOWClient::ESPNOWClient object
 * 
 * Allocates the array for holding our connected player objects.
 */
ESPNOW::ESPNOW(std::string mac_address) {
  m_mac_address = mac_address;
  m_connected_players = std::map<std::string, player_state_t>();
}

/**
 * @brief Destroy the ESPNOW::ESPNOW object
 * Ensures that the ESP scan stops on destructiion
 */
ESPNOW::~ESPNOW() {
  if (m_scan_task_handle != NULL) endScan();
}

/* -------------------------------------------------------------------------- */
/*                                  SCANNIING                                 */
/* -------------------------------------------------------------------------- */

/**
 * @brief Begins the FreeRTOS task for scanning for connected players.
 * 
 * Uses xTaskCreate to begin a FreeRTOS task, saving the handle locally to
 * the ESPNOWClient instance. This handle is later used for ending the task.
 */
void ESPNOW::beginScan() {
  if (m_scan_task_handle != NULL) endScan();
  m_connected_players.clear();
  xTaskCreate(
    Client::ESPNOW::_scanTaskImpl,
    "client_espnow SCAN",
    4,
    this,
    1,
    &m_scan_task_handle
  );
}

/**
 * @brief Ends the FreeRTOS task scanning for connected players.
 * 
 * Uses the handle from begin_scan to delete the task.
 */
void ESPNOW::endScan() {
  if (m_scan_task_handle == NULL) return;
  vTaskDelete(m_scan_task_handle);
  m_scan_task_handle = NULL;
}

/**
 * @brief 
 * 
 * @param pvParameter 
 */
void ESPNOW::_scanTaskImpl(void *_this) {
  ((ESPNOW*)_this)->_scanTask();
}

/**
 * @brief The infinite loop to listen for ESP-NOW peers
 * 
 * 
 */
void ESPNOW::_scanTask() {
  // ! TODO... set up ESP-NOW listening for peer connections
  while (true) {
    // ! TODO... check if have received any messages over ESP-NOW

    // pause for 100ms
    vTaskDelay(100);
  }
}

/**
 * @brief Sends a 
 * 
 * @param message 
 * @param mac_address 
 */
void ESPNOW::send(const char* message, uint8_t* mac_address) {
  
}

};