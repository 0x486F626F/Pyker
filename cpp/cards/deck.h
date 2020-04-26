#ifndef PYKER_DECK_H
#define PYKER_DECK_H


#include "card.h"


const uint8_t NUM_RANKS = 13;
const uint8_t NUM_SUITS = 4;


class Deck {
public:
    Deck();
    Card deal();
private:
    std::vector<Card> cards;
    size_t next_card_index = 0;
};


#endif //PYKER_DECK_H
