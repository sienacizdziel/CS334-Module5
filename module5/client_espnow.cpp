#include "include/client_espnow.h"
#include <functional>
#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

#define   MESH_PREFIX     "lightGame"
#define   MESH_PASSWORD   "123456"
#define   MESH_PORT       5555

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
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();

  
  ESP_ERROR_CHECK(esp_now_init());
}

/**
 * @brief Destroy the ESPNOW::ESPNOW object
 * Ensures that the ESP scan stops on destructiion
 */
ESPNOW::~ESPNOW() {
  mesh.stop();
  endScan();
}

/* -------------------------------------------------------------------------- */
/*                                  SCANNING                                 */
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
  while (true) {
    mesh.update();
  }
}


/**
 * @brief Sends an ESP-NOW message to a specific connected peers
 * 
 * @param message_type
 * @param message
 * @param mac_address 
 */
void send(ESPNOWEvent::EventType message_type, const char* message, uint8_t* mac_address) {
    ESPNOWEvent::msg.mac_address = mac_address;
    ESPNOWEvent::msg.message_type = message_type;
    ESPNOWEvent::msg.message = message;
    mesh.sendBroadcast((String) msg);
    taskSendMessage.setInterval(TASK_SECOND)
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}


};