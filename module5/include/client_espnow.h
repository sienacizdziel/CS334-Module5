#ifndef CLIENT_ESPNOW_H
#define CLIENT_ESPNOW_H

#include <map>
#include <stdbool.h>
#include <string>

#include "config.h"
#include "esp_now.h"
#include "painlessMesh.h"
#include "freeRTOS/FreeRTOS.h"
#include "freeRTOS/task.h"
#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

#define   MESH_PREFIX     "light_game"
#define   MESH_PASSWORD   "123456"
#define   MESH_PORT       5555

namespace cs334::Client {

namespace ESPNOWEvent {

/**
 * @brief An encoder for message event types
 *
 */
enum EventType {
  CONNECT, HEALTH, ASSIGN, IGNORE
};

/**
 * @brief A structure representing an panlessMesh message
 *
 * Struct includes message type and message buffer
 */
typedef struct esp_now_message_t {
  EventType message_type;     // 1 byte
  String message;
} esp_now_message_t;

} // namespace ESPNOWEvent

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

  void sendMessage();

  void sendSingle(uint32_t dest, EventType message_type, String &message_data);

  void setMessage(EventType message_type, String &message_data);

  /**
   * @brief xTask used to continually update m_connected_players with new
   * connections
   *
   * We use static so we can initiate this into its own xTaskCreate function.
   */
  void _scanTask(void);

  // Needed for painless library
  void receivedCallback( uint32_t from, String &msg );

  void newConnectionCallback(uint32_t nodeId);

  void changedConnectionCallback();

  void nodeTimeAdjustedCallback(int32_t offset);

public: // MEMBERS
  // save our mac address to be able to sort all MAC addresses
  std::string m_mac_address;
  // maps MAC addresses to player states
  std::map<std::string, player_state_t> m_connected_players;
  // message to send out
  esp_now_message_t msg_struct;

private: // MEMBERS
  TaskHandle_t m_scan_task_handle = NULL;
  Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );
  Scheduler userScheduler;
  painlessMesh mesh;

  /**
   * @brief Static wrapper around scan_task, used for starting the FreeRTOS task
   */
  static void _scanTaskImpl(void *_this);
};

}; // namespace cs334::Client

#endif /* CLIENT_ESPNOW_H */
