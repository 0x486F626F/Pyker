#ifndef PYKER_DECK_H
#define PYKER_DECK_H


#include "card.h"


const uint8_t NUM_RANKS = 13;
const uint8_t NUM_SUITS = 4;


class Deck {
public:
    Deck();
    t_card deal();
private:
    std::vector<Card> cards;
};


#endif //PYKER_DECK_H
