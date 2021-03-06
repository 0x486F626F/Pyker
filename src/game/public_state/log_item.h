#ifndef PYKER_LOG_ITEM_H
#define PYKER_LOG_ITEM_H


#include "cards/card.h"


/// Abstract base struct for different kinds of events that can be logged.
struct LogItem {
protected:
    /// Protected constructor to make LogItem abstract.
    LogItem() = default;
};

/// A log item for the start of a new game/hand.
struct GameStart : public LogItem {};

/// A log item for the start of a new betting round.
struct RoundStart : public LogItem {};

/// A log item for a player making a bet.
struct BetMade : public LogItem {
    size_t player_index;
    int amount;
    BetMade(size_t player_index, int amount) {
        this->player_index = player_index;
        this->amount = amount;
    }
};

/// A log item for a player folding
struct PlayerFold : public LogItem {
    size_t player_index;
    explicit PlayerFold(size_t player_index) { this->player_index = player_index; }
};

/// A log item indicating that a card has been added to the community cards.
struct CommunityCardRevealed : public LogItem {
    t_card card;
    explicit CommunityCardRevealed(t_card card) { this->card = card; }
};


#endif //PYKER_LOG_ITEM_H
