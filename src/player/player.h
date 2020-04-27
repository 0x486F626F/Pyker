#ifndef PYKER_PLAYER_H
#define PYKER_PLAYER_H


#include "cards/card.h"
#include "game/public_state.h"
#include "player_controller/player_controller.h"


class Player {
public:
    /// @param player_index the index of this player in the public state.
    explicit Player(PlayerController* controller, size_t player_index);
    void deal(Card card);
    /// Lets the player bet, check/call or raise.
    /// @return either the amount of chips that is added to the pot or FOLD, if the player folds.
    int do_betting_action(const PublicState& public_state, BetRange bet_range);
    /// Prepares the player for the start of a new game.
    void reset();

private:
    PlayerController* controller;
    /// The index of this player in the public state.
    size_t player_index;
    std::vector<Card> hand;

    void print_hand() const;
};


#endif //PYKER_PLAYER_H
