#ifndef CONFIG_H
#define CONFIG_H

#include <string>

/* -------------------------------------------------------------------------- */
/*                                CONFIGURATION                               */
/* -------------------------------------------------------------------------- */

// Configuration file for setting global constants
#define MAX_PLAYERS_N (10)

// GPIO pins of each peripheral
#define PIN_PHOTORESISTOR_1 (36)
#define PIN_PHOTORESISTOR_2 (39)
#define PIN_BUTTON (17)
#define PIN_LED_R (15)
#define PIN_LED_G (0)
#define PIN_LED_B (2)

// Game settings
#define TIME_HIDING_SECONDS (300)
#define TIME_PLAYING_SECONDS (300)

/* -------------------------------------------------------------------------- */
/*                                   TYPING                                   */
/* -------------------------------------------------------------------------- */

namespace cs334 {

/**
 * @brief The current state of the player this ESP is connected to
 */
typedef struct player_state {
  bool is_seeker;
  bool is_authoritative;
  uint16_t health;
  std::string mac_address;
} player_state_t;

}; // namespace cs334

#endif /* CONFIG_H */
