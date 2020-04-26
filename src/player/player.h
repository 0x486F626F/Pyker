#ifndef PYKER_PLAYER_H
#define PYKER_PLAYER_H


#include "cards/card.h"
#include "player_controller/player_controller.h"


class Player {
public:
    Player(PlayerController* controller, int initial_balance);
    void deal(Card card);
    /// Lets the player bet, check/call or raise.
    /// @return either the amount of chips that is added to the pot or FOLD, if the player folds.
    // TODO add trace parameter
    int do_betting_action(BetRange bet_range);
    /// Prepares the player for the start of a new game/hand.
    void reset();
    void reduce_balance(int amount);
    void set_balance(int b);
    bool has_folded() const;

private:
    PlayerController* controller;
    std::vector<Card> hand;
    int balance;
    bool folded = false;

    void print_hand() const;
};


#endif //PYKER_PLAYER_H
