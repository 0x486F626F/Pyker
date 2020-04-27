#ifndef PYKER_PLAYER_CONTROLLER_H
#define PYKER_PLAYER_CONTROLLER_H


#include "cards/card.h"
#include "game/public_state/public_state.h"


typedef std::tuple<int, int> BetRange;

/// Betting this number of chips is will make the character fold.
const int FOLD = -1;


/// Abstract base class for objects that control the behaviour of a player.
class PlayerController {
public:
    /// @return the amount of chips that the player bets or FOLD, if the player folds.
    [[nodiscard]] virtual int bet(t_card hand, const PublicState& public_state, BetRange bet_range) = 0;
    /// Sets the index of this player in the public state.
    void set_player_index(size_t index) { player_index = index; }

private:
    /// The index of this player in the public state.
    size_t player_index = -1;
};


#endif //PYKER_PLAYER_CONTROLLER_H
