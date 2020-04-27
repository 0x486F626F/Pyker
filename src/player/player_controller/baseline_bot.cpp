#include "baseline_bot.h"
#include "cards/hand.h"

#include <algorithm>


int BaselineBot::bet(t_card hand, const PublicState& public_state) {
    int min_bet = public_state.get_min_bet(player_index);
    int max_bet = public_state.get_balances()[player_index];

    // bet depending on the strength of the hand
    int strength = hand_strength(hand);
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

bool BaselineBot::fold_with_weak_hand() {
    return fold_with_weak_hand_distribution(rng);
}
