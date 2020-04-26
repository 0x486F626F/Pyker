#ifndef PYKER_PUBLIC_STATE_H
#define PYKER_PUBLIC_STATE_H


#include <cstddef>


struct PublicState {
    int big_blind;
    int small_blind;
    size_t dealer_index;
    // TODO history, pot, balances, public cards, which players have folded, ...
};


#endif //PYKER_PUBLIC_STATE_H
