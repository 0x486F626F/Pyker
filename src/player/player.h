#ifndef PYKER_PLAYER_H
#define PYKER_PLAYER_H


#include "cards/card.h"
#include "game/public_state.h"
#include "player_controller/player_controller.h"


class Player {
public:
    explicit Player(PlayerController* controller);
    void deal(Card card);
    /// Lets the player bet, check/call or raise.
    /// @return either the amount of chips that is added to the pot or FOLD, if the player folds.
    int do_betting_action(const PublicState& public_state, BetRange bet_range);
    /// Prepares the player for the start of a new game.
    void reset();

private:
    PlayerController* controller;
    std::vector<Card> hand;

    void print_hand() const;
};


#endif //PYKER_PLAYER_H
