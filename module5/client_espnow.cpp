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
ESPNOW::ESPNOW() {
  m_connected_players = std::map<uint32_t, player_state_t>();
  // prime the device for using ESP-NOW
  WiFi.mode(WIFI_STA);
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &ESPNOW::userScheduler, MESH_PORT );
  mesh.onReceive(&ESPNOW::receivedCallback);
  mesh.onNewConnection(&ESPNOW::newConnectionCallback);
  mesh.onChangedConnections(&ESPNOW::changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&ESPNOW::nodeTimeAdjustedCallback);

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
 * @brief Broadcasts Message
 * 
 * @param message_type
 * @param message
 * @param mac_address 
 */
void ESPNOW::sendMessage() {
  String message = msg_struct.message_type + msg_struct.message + mesh.getNodeId();
  mesh.sendBroadcast(message);
  taskSendMessage.setInterval(TASK_SECOND)
}

void ESPNOW::sendSingle(uint32_t dest, EventType message_type, String &message_data) {
  String message = message_type + &message_data + mesh.getNodeId();
  mesh.sendSingle(dest, message);
}

void ESPNOW::sendBroadcast(EventType message_type, String &message_data) {
  msg_struct.message_type = message_type;
  msg_struct.message = &message_data;
}

std::list<uint32_t> ESPNOW::getConnectedPlayers() {
  return mesh.getNodeList();
}

uint32_t ESPNOW::getNodeId() {
  return mesh.getNodeId();
}

// Needed for painless library
void ESPNOW::receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

void ESPNOW::newConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
  if (m_player.is_authoritative == true) {
      player_state newPlayer{.nodeID = nodeId, .is_authoritative = false};
      m_connected_players.insert(pair<uint32_t, player_state>(nodeId, newPlayer));
  }
}

void ESPNOW::changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void ESPNOW::nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}


};