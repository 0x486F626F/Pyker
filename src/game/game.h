#ifndef PYKER_GAME_H
#define PYKER_GAME_H


#include "player/player.h"
#include "game/public_state/public_state.h"


/// Plays one game/hand, consisting of up to four betting rounds.
/// Assumes that players.size() >= 2.
void play_hand(std::vector<Player>& players, PublicState& public_state);


#endif //PYKER_GAME_H
