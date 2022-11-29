#include "include/client_espnow.h"

#include <functional>
#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

namespace cs334::Client {

/**
 * @brief Construct a new ESPNOWClient::ESPNOWClient object
 *
 * Allocates the array for holding our connected player objects.
 */
ESPNOW::ESPNOW(std::string mac_address) {
  m_mac_address = mac_address;
  m_connected_players = std::map<std::string, player_state_t>();
  // prime the device for using ESP-NOW
  WiFi.mode(WIFI_STA);
  ESP_ERROR_CHECK(esp_now_init());
}

/**
 * @brief Destroy the ESPNOW::ESPNOW object
 * Ensures that the ESP scan stops on destructiion
 */
ESPNOW::~ESPNOW() {
  endScan();
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
 * Should continually listen for "connect" messages from peers. For any peers'
 * mac addresses that do not exist in our m_connected_players map, we should
 * add them to the m_connected_players map.
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
 * @brief Sends an ESP-NOW message to a specific connected peers
 *
 * @param message_type
 * @param message
 * @param mac_address
 */
void ESPNOW::send(ESPNOWEvent::EventType message_type, const char* message, uint8_t* mac_address) {

}

/**
 * @brief Broadcasts an ESP-NOW message to all connected peers
 *
 * @param message_type
 * @param message
 */
void send(ESPNOWEvent::EventType message_type, const char* message) {

}

};
