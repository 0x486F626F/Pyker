#ifndef PYKER_SESSION_H
#define PYKER_SESSION_H


#include "player/player.h"
#include "game/public_state/public_state.h"


/// A session consisting of multiple games/hands.
class Session {
public:
    Session(std::vector<Player> players, int starting_balance, int big_blind = 20, int small_blind = 10);
    void start();
private:
    std::vector<Player> players;
    PublicState public_state;
};


#endif //PYKER_SESSION_H
