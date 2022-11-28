#ifndef STATE_RANKING_H
#define STATE_RANKING_H

#include "game.h"
#include "state_interface.h"

namespace cs334 {

class RankingState : public GameState {
public:
  RankingState(Game *game) : GameState("ranking", game) {}
  void setup() override;
  void run() override;

private:
};

}; // namespace cs334

#endif /* STATE_RANKING_H */
