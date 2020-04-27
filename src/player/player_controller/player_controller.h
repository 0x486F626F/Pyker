#ifndef PYKER_PLAYER_CONTROLLER_H
#define PYKER_PLAYER_CONTROLLER_H


#include "cards/card.h"


typedef std::tuple<int, int> BetRange;

/// Betting this number of chips is will make the character fold.
const int FOLD = -1;


/// An interface for objects that control the behaviour of a player.
class PlayerController {
public:
    /// @return the amount of chips that the player bets or FOLD, if the player folds.
    // TODO add public state parameter
    [[nodiscard]] virtual int bet(std::vector<Card> hand, BetRange bet_range) = 0;
};


#endif //PYKER_PLAYER_CONTROLLER_H
