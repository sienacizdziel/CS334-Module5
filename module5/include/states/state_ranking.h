#ifndef STATE_RANKING_H
#define STATE_RANKING_H

#include "state_interface.h"

namespace cs334 {

class Game;

class RankingState : public GameState {
 public:
  RankingState(Game* game) : GameState("ranking", game) {}
  void setup() override;
  void run() override;

 private:
};

};

#endif /* STATE_RANKING_H */
