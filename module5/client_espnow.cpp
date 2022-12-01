#include "include/client_espnow.h"

#include <functional>
#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

namespace cs334::Client {

static bool is_authoritative = false;
static TaskHandle_t scan_task_handle = NULL;
static std::map<uint32_t, player_state_t> connected_players;
static Scheduler userScheduler;
static painlessMesh mesh;
static Task *taskSendMessage = NULL;
static ESPNOWEvent::esp_now_message_t msg_struct;

/* -------------------------------------------------------------------------- */
/*                                  LISTENERS                                 */
/* -------------------------------------------------------------------------- */

/**
 * @brief On message received
 *
 * @param from
 * @param msg
 */
static void pm_receivedCallback(uint32_t from, String &msg) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

/**
 * @brief On new connection
 *
 * @param nodeId
 */
static void pm_newConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
  if (is_authoritative == true) {
    player_state newPlayer{
        .is_seeker = false,
        .is_authoritative = false,
        .health = 0,
        .node_id = nodeId,
    };
    connected_players.insert(std::pair<uint32_t, player_state>(nodeId, newPlayer));
  }
}

/**
 * @brief On Connectino changed
 *
 */
static void pm_changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

/**
 * @brief On node time adjusted
 *
 * @param offset
 */
static void pm_nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

/**
 * @brief Broadcasts Message
 *
 * @param message_type
 * @param message
 * @param mac_address
 */
static void pm_sendMessage() {
  String message = msg_struct.message_type + msg_struct.message + mesh.getNodeId();
  mesh.sendBroadcast(message);
  taskSendMessage->setInterval(TASK_SECOND);
}

/**
 * @brief The infinite loop to listen for ESP-NOW peers
 *
 * Should continually listen for "connect" messages from peers. For any peers'
 * mac addresses that do not exist in our m_connected_players map, we should
 * add them to the m_connected_players map.
 */
static void pm_scanTask(void *pvParameter) {
  while (true) {
    mesh.update();
  }
}

/* -------------------------------------------------------------------------- */
/*                                ESPNOW CLIENT                               */
/* -------------------------------------------------------------------------- */

/**
 * @brief Construct a new ESPNOWClient::ESPNOWClient object
 *
 * Allocates the array for holding our connected player objects.
 */
void ESPNOW::setup(bool p_is_authoritative) {
  // prime the device for using ESP-NOW
  WiFi.mode(WIFI_STA);

  // set up the painless mesh with all the listeners
  mesh.setDebugMsgTypes(ERROR | STARTUP);  // set before init() so that you can see startup messages
  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(pm_receivedCallback);
  mesh.onNewConnection(pm_newConnectionCallback);
  mesh.onChangedConnections(pm_changedConnectionCallback);
  mesh.onNodeTimeAdjusted(pm_nodeTimeAdjustedCallback);

  // set up the message sending task
  taskSendMessage = new Task(TASK_SECOND, TASK_FOREVER, pm_sendMessage);
  userScheduler.addTask(*taskSendMessage);
  taskSendMessage->enable();

  // sync authoritative state
  is_authoritative = p_is_authoritative;

  // init ESP NOW
  ESP_ERROR_CHECK(esp_now_init());
}

/**
 * @brief Destroy the ESPNOW::ESPNOW object
 * Ensures that the ESP scan stops on destructiion
 */
void ESPNOW::destroy() {
  mesh.stop();
  ESPNOW::endScan();
  delete taskSendMessage;
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
  if (scan_task_handle != NULL) endScan();
  connected_players.clear();
  xTaskCreate(
      pm_scanTask,
      "client_espnow SCAN",
      4096,
      NULL,
      1,
      &scan_task_handle);
}

/**
 * @brief Ends the FreeRTOS task scanning for connected players.
 *
 * Uses the handle from begin_scan to delete the task.
 */
void ESPNOW::endScan() {
  if (scan_task_handle == NULL) return;
  vTaskDelete(scan_task_handle);
  scan_task_handle = NULL;
}

/* -------------------------------------------------------------------------- */
/*                              SENDING MESSAGES                              */
/* -------------------------------------------------------------------------- */

/**
 * @brief Sets message to be sent to a single connected player
 *
 * @param dest
 * @param message_type
 * @param message_data
 */
void ESPNOW::sendSingle(uint32_t dest, ESPNOWEvent::EventType message_type, String &message_data) {
  String message = message_type + message_data + mesh.getNodeId();
  mesh.sendSingle(dest, message);
}

/**
 * @brief Sets message to be broadcasted to all connected players
 *
 * @param message_type
 * @param message_data
 */
void ESPNOW::sendBroadcast(ESPNOWEvent::EventType message_type, String &message_data) {
  msg_struct.message_type = message_type;
  msg_struct.message = message_data;
}

/* -------------------------------------------------------------------------- */
/*                                   HELPERS                                  */
/* -------------------------------------------------------------------------- */

/**
 * @brief Returns all connected players
 *
 * @return std::list<uint32_t>
 */
std::list<uint32_t> ESPNOW::getConnectedPlayers() {
  return mesh.getNodeList();
}

/**
 * @brief Returns own node ID
 *
 * @return uint32_t
 */
uint32_t ESPNOW::getNodeId() {
  return mesh.getNodeId();
}

};  // namespace cs334::Client
