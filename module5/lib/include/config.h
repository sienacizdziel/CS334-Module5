#ifndef CONFIG_H
#define CONFIG_H

#include <string>

// Configuration file for setting global constants
#define MAX_PLAYERS_N (10)

/* -------------------------------------------------------------------------- */
/*                                   TYPING                                   */
/* -------------------------------------------------------------------------- */

/**
 * @brief The current state of the player this ESP is connected to
 */
typedef struct player_state {
  bool is_seeker;
  bool is_authoritative;
  uint16_t health;
  std::string mac_address;
} player_state_t;


#endif /* CONFIG_H */
