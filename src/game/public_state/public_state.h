#ifndef PYKER_PUBLIC_STATE_H
#define PYKER_PUBLIC_STATE_H


#include "cards/card.h"
#include "log_item.h"

#include <memory>


/// Public information about the game and the players.
/// Always modify the struct through its own methods if possible, since these actions will be added to the log.
class PublicState {
public:
    PublicState(size_t num_players, int small_blind, int big_blind, int starting_balance);

    /// Resets bets, folded and community_cards. Moves dealer_index to the next non-bankrupt player.
    void start_game();
    /// Logs the start of a new betting round.
    void start_round();
    void reveal_community_card(t_card card);
    /// Reduces a player's balance by the specified amount of chips and adds it to their bet.
    /// If the player doesn't have enough chips left, they bet all of their remaining chips.
    /// @return the amount of chips bet.
    int bet(size_t player_index, int amount);
    void fold(size_t player_index);
    void clear_bets();

    /// @return the indices of all players that aren't bankrupt.
    ///         A player is considered bankrupt if their balance is 0 and they haven't bet any chips this game.
    [[nodiscard]] std::vector<size_t> get_remaining_player_indices() const;
    /// @return balances of the non-bankrupt players.
    [[nodiscard]] std::vector<int> get_remaining_balances() const;
    /// @return the amount of chips bet in the current game by each non-bankrupt player.
    [[nodiscard]] std::vector<int> get_remaining_bets() const;
    /// @return the indices of all players who folded this game.
    [[nodiscard]] std::vector<size_t> get_folded_player_indices() const;
    /// @return the index of the next non-bankrupt player after the given index.
    [[nodiscard]] size_t next_player_after(size_t player_index) const;
    [[nodiscard]] size_t get_small_blind_index() const;
    [[nodiscard]] size_t get_big_blind_index() const;
    /// The given player must bet at least this much or fold.
    [[nodiscard]] int get_min_bet(size_t player_index) const;
    /// The highest amount any player has bet so far (NOT the opposite of get_min_bet).
    [[nodiscard]] int get_highest_bet() const;

    // basic getters
    [[nodiscard]] int get_big_blind() const { return big_blind; }
    [[nodiscard]] int get_small_blind() const { return small_blind; }
    [[nodiscard]] int get_dealer_index() const { return dealer_index; }
    /// The amount of chips each of the players has left.
    /// Use get_remaining_balances() for the balances of non-bankrupt players.
    [[nodiscard]] const std::vector<int>& get_balances() const { return balances; }
    /// The amount of chips bet by each player in the current game.
    /// Same size as balances.
    /// Use get_remaining_bets() for the bets of non-bankrupt players.
    [[nodiscard]] const std::vector<int>& get_bets() const { return bets; }
    /// For each player either true, if they have folded this game, or false, otherwise.
    /// Same size as balances.
    [[nodiscard]] const std::vector<bool>& get_folded() const { return folded; }
    [[nodiscard]] t_card get_community_cards() const { return community_cards; }
    [[nodiscard]] const std::vector<std::unique_ptr<LogItem>>& get_log() const { return log; }

private:
    int big_blind;
    int small_blind;
    size_t dealer_index;
    /// The amount of chips each of the players has left.
    /// Use get_remaining_balances() for the balances of non-bankrupt players.
    std::vector<int> balances;
    /// The amount of chips bet by each player in the current game.
    /// Same size as balances.
    std::vector<int> bets;
    /// For each player either true, if they have folded this game, or false, otherwise.
    /// Same size as balances.
    std::vector<bool> folded;
    t_card community_cards = 0;
    std::vector<std::unique_ptr<LogItem>> log;
};


#endif //PYKER_PUBLIC_STATE_H
