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

Card Deck::deal() {
    if (cards.empty()) {
        throw std::length_error("There are no cards left in the deck");
    }
    Card card = cards.back();
    cards.pop_back();
    return card;
}
