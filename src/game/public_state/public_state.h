#ifndef PYKER_PUBLIC_STATE_H
#define PYKER_PUBLIC_STATE_H


#include "cards/card.h"
#include "log_item.h"


/// Public information about the game and the players.
/// Always modify the struct through its own methods if possible, since these actions will be added to the log.
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
    std::vector<Card> community_cards;
    size_t dealer_index;
    // TODO document
    std::vector<LogItem*> log;

    /// Resets bets, folded and community_cards. Moves dealer_index to the next non-bankrupt player.
    void start_game();
    /// Logs the start of a new betting round.
    void start_round();
    void reveal_community_card(Card card);
    /// Reduces a player's balance by the specified amount of chips and adds it to their bet.
    /// If the player doesn't have enough chips left, they bet all of their remaining chips.
    /// @return the amount of chips bet.
    int bet(size_t player_index, int amount);
    /// @return the indices of all players that aren't bankrupt.
    ///         A player is considered bankrupt if their balance is 0 and they haven't bet any chips this game.
    [[nodiscard]] std::vector<size_t> remaining_player_indices() const;
    /// @return balances of the non-bankrupt players.
    [[nodiscard]] std::vector<int> remaining_balances() const;
    /// @return the amount of chips bet in the current game by each non-bankrupt player.
    [[nodiscard]] std::vector<int> remaining_bets() const;
    /// @return for each non-bankrupt player: either true, if they have folded this game, or false, otherwise.
    [[nodiscard]] std::vector<bool> remaining_folded() const;
    [[nodiscard]] size_t small_blind_index() const;
    [[nodiscard]] size_t big_blind_index() const;
private:
    /// @return the index of the next non-bankrupt player after the given index.
    [[nodiscard]] size_t next_player_after(size_t player_index) const;
};


#endif //PYKER_PUBLIC_STATE_H
