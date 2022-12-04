#include "include/client_espnow.h"

#include <functional>
#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

namespace cs334::Client {

static void pm_sendMessage();

static bool is_authoritative = false;
static TaskHandle_t scan_task_handle = NULL;
static Scheduler userScheduler;
static painlessMesh mesh;
static Task taskSendMessage(TASK_SECOND * 1, TASK_FOREVER, &pm_sendMessage);
static std::map<uint32_t, player_state_t> *players = NULL;
static player_state_t *player = NULL;
static bool has_seeker = false;
static bool begin_game = false;
static bool has_rank = false;
static ESPNOWEvent::esp_now_message_t msg_struct{
    .message_type = ESPNOWEvent::EventType::IGNORE,
    .message = 43770};

/* -------------------------------------------------------------------------- */
/*                                  LISTENERS                                 */
/* -------------------------------------------------------------------------- */

/**
 * @brief On message received
 *
 * @param from
 * @param msg
 */
static void pm_receivedCallback(uint32_t from, String &in) {
  if (in == NULL) return;
  ESPNOWEvent::esp_now_message_t msg = {};
  if (!sscanf((char *)in.c_str(), "%u %u", &msg.message_type, &msg.message)) return;
  switch (msg.message_type) {
    case ESPNOWEvent::EventType::ASSIGN: {
      Serial.printf("[ESP-NOW] Received message from %u: the seeker is %u\n", from, msg.message);
      if (msg.message == mesh.getNodeId()) {
        player->is_seeker = true;
      }
      has_seeker = true;
    } break;
    case ESPNOWEvent::EventType::HEALTH: {
      if (player->is_authoritative == true) {
        (*players)[from].health = msg.message;
      }
    } break;
    case ESPNOWEvent::EventType::RANK: {
      if (msg.message == mesh.getNodeId()) {
        player->is_winner = true;
      }
      has_seeker = true;
    } break;
    case ESPNOWEvent::EventType::BEGIN_GAME: {
      begin_game = true;
    } break;
    default:
      break;
  }
}

/**
 * @brief On new connection
 *
 * @param nodeId
 */
static void pm_newConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
  if (is_authoritative == true && players != NULL) {
    player_state newPlayer{
        .is_seeker = false,
        .is_authoritative = false,
        .health = 0,
        .is_winner = false,
    };
    players->insert(std::pair<uint32_t, player_state>(nodeId, newPlayer));
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
  // Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

/**
 * @brief Broadcasts Message
 *
 * @param message_type
 * @param message
 * @param mac_address
 */
static void pm_sendMessage() {
  char msg[64];
  sprintf(msg, "%d %u", msg_struct.message_type, msg_struct.message);
  mesh.sendBroadcast(msg);
  taskSendMessage.setInterval(TASK_SECOND * 1);
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
void ESPNOW::setup(player_state_t *p_player, std::map<uint32_t, player_state_t> *p_players) {
  // sync authoritative state
  has_seeker = false;
  begin_game = false;
  has_rank = false;
  players = p_players;
  player = p_player;

  // prime the device for using ESP-NOW
  // set up the painless mesh with all the listeners
  mesh.setDebugMsgTypes(ERROR | STARTUP);
  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&pm_receivedCallback);
  mesh.onChangedConnections(&pm_changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&pm_nodeTimeAdjustedCallback);

  // set up the message sending task
  userScheduler.addTask(taskSendMessage);
  taskSendMessage.enable();

  // log own node id
  Serial.printf("[ESP-NOW] Device node id: %u\n", mesh.getNodeId());
}

/**
 * @brief Destroy the ESPNOW::ESPNOW object
 * Ensures that the ESP scan stops on destructiion
 */
void ESPNOW::destroy() {
  mesh.stop();
  ESPNOW::endScan();
}

/**
 * @brief Sets if the ESP is authoritative
 */
void ESPNOW::setIsAuthoritative(bool val) {
  is_authoritative = val;
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
void ESPNOW::sendSingle(uint32_t dest, ESPNOWEvent::EventType message_type, uint32_t message_data) {
  String message = message_type + " " + message_data;
  mesh.sendSingle(dest, message);
}

/**
 * @brief Sets message to be broadcasted to all connected players
 *
 * @param message_type
 * @param message_data
 */
void ESPNOW::sendBroadcast(ESPNOWEvent::EventType message_type, uint32_t message_data) {
  Serial.printf("[ESP-NOW] Set broadcast to: %d %u\n", message_type, message_data);
  msg_struct.message_type = message_type;
  msg_struct.message = message_data;
}

/**
 * @brief Sets whether or not you accept new connections
 *
 * @param val
 */
void ESPNOW::setAcceptingNewConnections(bool val) {
  if (val) {
    mesh.onNewConnection(&pm_newConnectionCallback);
  } else {
    mesh.onNewConnection(NULL);
  }
}

/* -------------------------------------------------------------------------- */
/*                                   HELPERS                                  */
/* -------------------------------------------------------------------------- */

/**
 * @brief Checks whether or not the ESP-NOW has received the seeker message
 *
 * @return true
 * @return false
 */
bool ESPNOW::hasSeeker() {
  return has_seeker;
}

/**
 * @brief Checks whether or not the ESP-NOW has received the rank message
 *
 * @return true
 * @return false
 */
bool ESPNOW::hasRank() {
  return has_rank;
}

/**
 * @brief Checks whether or not the ESP-NOW has received the begin_game message
 *
 * @return true
 * @return false
 */
bool ESPNOW::shouldBeginGame() {
  return begin_game;
}

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
