#ifndef CLIENT_ESPNOW_H
#define CLIENT_ESPNOW_H

#include <map>
#include <stdbool.h>
#include <string>

#include "config.h"
#include "esp_now.h"
#include "freeRTOS/FreeRTOS.h"
#include "freeRTOS/task.h"
#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

namespace cs334::Client {

namespace ESPNOWEvent {

/**
 * @brief An encoder for message event types
 *
 */
enum EventType { CONNECT, HEALTH };

/**
 * @brief A structure representing an ESP-NOW message
 *
 * This is aligned to as to safely fall under the 250 byte limit of ESP-NOW
 * messages. This allows us to parse a mac address, message type, and variable
 * byte-encoded message data from the message.
 */
typedef struct esp_now_message_t {
  uint8_t mac_address[8];     // 8 bytes
  EventType message_type;     // 1 byte
  uint8_t message_event[128]; // 128 bytes
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

  /**
   * @brief Sends an ESP-NOW message to a specific MAC address.
   *
   * Note that ESP-NOW messages are limited in size, to something like 256
   * bytes. So we should make sure to keep our messages super short.
   *
   * @param mac_address
   */
  void send(ESPNOWEvent::EventType message_type, const char *message,
            uint8_t *mac_address);

  /**
   * @brief Broadcasts an ESP-NOW message to all connected players.
   *
   * This function will be used by the authoritative node
   *
   * @param mac_address
   */
  void send(ESPNOWEvent::EventType message_type, const char *message);

  /**
   * @brief xTask used to continually update m_connected_players with new
   * connections
   *
   * We use static so we can initiate this into its own xTaskCreate function.
   */
  void _scanTask(void);

public: // MEMBERS
  // save our mac address to be able to sort all MAC addresses
  std::string m_mac_address;
  // maps MAC addresses to player states
  std::map<std::string, player_state_t> m_connected_players;

  // @Srikar this should be dynamically updated from the authoritative node
  // with the MAC address of currently connected members (I think)
  std::vector<std::string> m_authoritative_connected_nodes;


private: // MEMBERS
  TaskHandle_t m_scan_task_handle = NULL;

  /**
   * @brief Static wrapper around scan_task, used for starting the FreeRTOS task
   */
  static void _scanTaskImpl(void *_this);
};

}; // namespace cs334::Client

#endif /* CLIENT_ESPNOW_H */
