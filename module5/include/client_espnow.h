#ifndef CLIENT_ESPNOW_H
#define CLIENT_ESPNOW_H

#include <stdbool.h>

#include <chrono>
#include <map>
#include <string>
#include <vector>

#include "config.h"
#include "esp_now.h"
#include "freeRTOS/FreeRTOS.h"
#include "freeRTOS/task.h"
#include "painlessMesh.h"
#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

#define MESH_PREFIX "light_game"
#define MESH_PASSWORD "123456"
#define MESH_PORT 5555

namespace cs334::Client {

namespace ESPNOWEvent {

/**
 * @brief An encoder for message event types
 *
 */
enum EventType {
  CONNECT,
  HEALTH,
  ASSIGN,
  IGNORE
};

/**
 * @brief A structure representing an panlessMesh message
 *
 * Struct includes message type and message buffer
 */
typedef struct esp_now_message_t {
  EventType message_type;  // 1 byte
  String message;
} esp_now_message_t;

}  // namespace ESPNOWEvent

namespace ESPNOW {

/**
 * @brief
 *
 * @param is_authoritative
 */
void setup(bool is_authoritative = false);
void destroy();

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

void sendSingle(uint32_t dest, ESPNOWEvent::EventType message_type, String &message_data);
void sendBroadcast(ESPNOWEvent::EventType message_type, String &message_data);
static void sendMessage();
std::list<uint32_t> getConnectedPlayers();
uint32_t getNodeId();

}  // namespace ESPNOW

};  // namespace cs334::Client

#endif /* CLIENT_ESPNOW_H */
