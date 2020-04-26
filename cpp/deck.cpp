#include "deck.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>


Deck::Deck() {
    // create a standard 52 card deck
    for (uint8_t suit = 0; suit < NUM_SUITS; suit++) {
        for (uint8_t rank = 0; rank < NUM_RANKS; rank++) {
            cards.emplace_back(1ll << (suit + rank * 4));
        }
    }

    // shuffle the cards
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(cards.begin(), cards.end(), std::default_random_engine(seed));
}

Card* Deck::deal() {
    if (next_card_index >= NUM_SUITS * NUM_RANKS) {
        // the deck is empty
        return nullptr;
    }

    next_card_index++;
    return &cards[next_card_index - 1];
}
