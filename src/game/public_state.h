#ifndef PYKER_PUBLIC_STATE_H
#define PYKER_PUBLIC_STATE_H


#include "cards/card.h"


/// Public information about the game and the players.
struct PublicState {
    int big_blind;
    int small_blind;
    /// The amount of chips each of the players has left.
    /// Use remaining_balances() for the balances of non-bankrupt players.
    std::vector<int> balances;
    /// The amount of chips bet by each player in the current game.
    /// Same size as balances.
    /// Use remaining_bets() for the bets of non-bankrupt players.
    std::vector<int> bets;
    /// For each player either true, if they have folded this game, or false, otherwise.
    /// Same size as balances.
    /// Use remaining_folded() to get this information only for non-bankrupt players.
    std::vector<bool> folded;
    std::vector<Card> public_cards;
    size_t dealer_index;
    // TODO history of betting actions

    /// Resets bets, folded and public_cards. Moves dealer_index to the next non-bankrupt player.
    void start_game();
    /// @return the indices of all players that aren't bankrupt.
    ///         A player is considered bankrupt if their balance is 0 and they haven't bet any chips this game.
    [[nodiscard]] std::vector<size_t> remaining_player_indices() const;
    /// @return balances of the non-bankrupt players.
    [[nodiscard]] std::vector<int> remaining_balances() const;
    /// @return the amount of chips bet in the current game by each non-bankrupt player.
    [[nodiscard]] std::vector<int> remaining_bets() const;
    /// @return for each non-bankrupt player: either true, if they have folded this game, or false, otherwise.
    [[nodiscard]] std::vector<bool> remaining_folded() const;
};


#endif //PYKER_PUBLIC_STATE_H
