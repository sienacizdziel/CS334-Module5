#include "client_espnow.h"

using namespace cs334;

/**
 * @brief Construct a new ESPNOWClient::ESPNOWClient object
 * 
 * Allocates the array for holding our connected player objects.
 */
ESPNOWClient::ESPNOWClient() {
  m_connected_players = malloc(sizeof(player_state_t) * MAX_PLAYERS_N);
  m_connected_players_n = 0;
}

/**
 * @brief Begins the FreeRTOS task for scanning for connected players.
 * 
 * Uses xTaskCreate to begin a FreeRTOS task, saving the handle locally to
 * the ESPNOWClient instance. This handle is later used for ending the task.
 */
void ESPNOWClient::begin_scan() {
  if (m_scan_task_handle != NULL) end_scan();
  m_connected_players = c
  xTaskCreate(
    std::bind(ESPNOWClient::scan_task, this),
    "client_espnow SCAN",
    4,
    NULL,
    1,
    &m_scan_task_handle
  )
}

/**
 * @brief Ends the FreeRTOS task scanning for connected players.
 * 
 * Uses the handle from begin_scan to delete the task.
 */
void ESPNOWClient::end_scan() {
  if (m_scan_task_handle == NULL) return;
  vTaskDelete(m_scan_task_handle);
}

/**
 * @brief 
 * 
 * @param pvParameter 
 */
void ESPNOWClient::scan_task(void *pvParameter) {
  //! TODO: Implement an infinite loop that scans for players and adds
  // them to the m_connected_players array if they are not already in it.
}

/**
 * @brief Sends a 
 * 
 * @param message 
 * @param mac_address 
 */
void ESPNOWClient::send(const char* message, uint8_t* mac_address) {
  
}