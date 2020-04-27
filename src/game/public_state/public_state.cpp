#include "public_state.h"

#include <algorithm>


// private

template <typename T>
/// @return the elements of vec at the specified indices
std::vector<T> vector_subset(const std::vector<T>& vec, const std::vector<size_t>& indices) {
    std::vector<T> subset;
    for (size_t index : indices) {
        subset.push_back(vec[index]);
    }
    return subset;
}

size_t PublicState::next_player_after(size_t player_index) const {
    size_t index = (player_index + 1) % balances.size();
    while (balances[index] <= 0 && bets[index] <= 0) {
        index = (index + 1) % balances.size();
    }
    return index;
}


// public

PublicState::PublicState(size_t num_players, int small_blind, int big_blind, int starting_balance) {
    this->small_blind = small_blind;
    this->big_blind = big_blind;
    dealer_index = 0,  // TODO choose dealer randomly
    balances = std::vector(num_players, starting_balance);
    bets = std::vector(num_players, 0);
    folded = std::vector(num_players, false);
}

void PublicState::start_game() {
    bets = std::vector(balances.size(), 0);
    folded = std::vector(balances.size(), false);
    community_cards = 0;
    dealer_index = next_player_after(dealer_index);
    log.push_back(std::make_unique<GameStart>(GameStart()));
}

void PublicState::start_round() {
    log.push_back(std::make_unique<RoundStart>(RoundStart()));
}

void PublicState::reveal_community_card(t_card card) {
    community_cards = community_cards & card;
    log.push_back(std::make_unique<CommunityCardRevealed>(CommunityCardRevealed(card)));
}

int PublicState::bet(size_t player_index, int amount) {
    amount = std::clamp(amount, 0, balances[player_index]);
    balances[player_index] -= amount;
    bets[player_index] += amount;
    log.push_back(std::make_unique<BetMade>(BetMade(player_index, amount)));
    return amount;
}

std::vector<size_t> PublicState::get_remaining_player_indices() const {
    std::vector<size_t> indices;

    for (int i = 0; i < balances.size(); i++) {
        if (balances[i] > 0 || bets[i] > 0) {
            indices.push_back(i);
        }
    }

    return indices;
}

std::vector<int> PublicState::get_remaining_balances() const {
    return vector_subset(balances, get_remaining_player_indices());
}

std::vector<int> PublicState::get_remaining_bets() const {
    return vector_subset(bets, get_remaining_player_indices());
}

std::vector<bool> PublicState::get_remaining_folded() const {
    return vector_subset(folded, get_remaining_player_indices());
}

size_t PublicState::get_small_blind_index() const {
    return next_player_after(dealer_index);
}

size_t PublicState::get_big_blind_index() const {
    return next_player_after(next_player_after(dealer_index));
}
