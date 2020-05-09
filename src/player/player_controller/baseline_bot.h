#ifndef PYKER_BASELINE_BOT_H
#define PYKER_BASELINE_BOT_H


#include "player_controller.h"

#include <random>


/// A simple bot that bets based on the strength of its hand.
/// TODO doesn't work at the moment
class BaselineBot : public PlayerController {
public:
    int bet(t_card hand, const PublicState& public_state) override;

private:
    std::bernoulli_distribution fold_with_weak_hand_distribution = std::bernoulli_distribution(0.5);
    // TODO use std::mt19937 instead of the default random engine
    std::default_random_engine rng = std::default_random_engine(std::random_device()());
    /// Randomly decides whether the player should keep playing despite having a bad hand
    bool fold_with_weak_hand();
};


#endif //PYKER_BASELINE_BOT_H
