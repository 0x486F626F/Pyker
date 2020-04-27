#include "baseline_bot.h"
#include "cards/hand.h"

#include <algorithm>


int BaselineBot::bet(std::vector<Card> hand, const PublicState& public_state, BetRange bet_range) {
    auto [min_bet, max_bet] = bet_range;

    // bet depending on the strength of the hand
    int strength = hand_strength(encode_cards(hand));
    int bet = strength * 10;

    if (bet < min_bet && fold_with_weak_hand()) {
        bet = FOLD;
    }

    // clamp bet to range
    if (bet != FOLD) {
        bet = std::clamp(bet, min_bet, max_bet);
    }

    return bet;
}

/// Randomly decides whether the player should keep playing despite having a bad hand
bool BaselineBot::fold_with_weak_hand() {
    return fold_with_weak_hand_distribution(rng);
}
