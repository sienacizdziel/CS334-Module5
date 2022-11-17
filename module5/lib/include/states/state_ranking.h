#ifndef STATE_RANKING_H
#define STATE_RANKING_H

#include "states/state_interface.h"

namespace cs334 {

class RankingState : GameState {
 public:
  RankingState(player_state_t* player_state) : GameState("ranking", player_state) {}
  void setup() override;
  void run() override;

 private:
};

};

#endif /* STATE_RANKING_H */
