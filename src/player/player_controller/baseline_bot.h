#ifndef PYKER_BASELINE_BOT_H
#define PYKER_BASELINE_BOT_H


#include "player_controller.h"

#include <random>


/// A simple bot that bets based on the strength of its hand.
class BaselineBot : PlayerController {
public:
    int bet(std::vector<Card> hand, const PublicState& public_state, BetRange bet_range) override;

private:
    std::bernoulli_distribution fold_with_weak_hand_distribution = std::bernoulli_distribution(0.5);
    std::default_random_engine rng = std::default_random_engine(std::random_device()());
    bool fold_with_weak_hand();
};


#endif //PYKER_BASELINE_BOT_H
