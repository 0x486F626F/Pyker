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


// public

PublicState::PublicState(size_t num_players, int small_blind, int big_blind, int starting_balance) {
    this->small_blind = small_blind;
    this->big_blind = big_blind;
    dealer_index = 1,  // TODO choose dealer randomly
    balances = std::vector(num_players, starting_balance);
    bets = std::vector(num_players, 0);
    folded = std::vector(num_players, false);
}

void PublicState::start_game() {
    folded = std::vector(balances.size(), false);
    community_cards = 0;
    dealer_index = next_player_after(dealer_index);
    log.push_back(std::make_unique<GameStart>(GameStart()));
}

void PublicState::start_round() {
    log.push_back(std::make_unique<RoundStart>(RoundStart()));
}

void PublicState::reveal_community_card(t_card card) {
    community_cards = community_cards | card;
    log.push_back(std::make_unique<CommunityCardRevealed>(CommunityCardRevealed(card)));
}

int PublicState::bet(size_t player_index, int amount) {
    amount = std::clamp(amount, 0, balances[player_index]);
    balances[player_index] -= amount;
    bets[player_index] += amount;
    log.push_back(std::make_unique<BetMade>(BetMade(player_index, amount)));
    return amount;
}

void PublicState::fold(size_t player_index) {
    folded[player_index] = true;
    log.push_back(std::make_unique<PlayerFold>(PlayerFold(player_index)));
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


std::vector<size_t> PublicState::get_folded_player_indices() const {
    std::vector<size_t> indices;
    for (int i = 0; i < folded.size(); i++) {
        if (folded[i]) { indices.push_back(i); }
    }
    return indices;
}

size_t PublicState::next_player_after(size_t player_index) const {
    size_t index = (player_index + 1) % balances.size();
    while (balances[index] <= 0 && bets[index] <= 0) {
        index = (index + 1) % balances.size();
    }
    return index;
}

size_t PublicState::get_small_blind_index() const {
    return next_player_after(dealer_index);
}

size_t PublicState::get_big_blind_index() const {
    return next_player_after(next_player_after(dealer_index));
}

int PublicState::get_min_bet(size_t player_index) const {
    // the player must at least catch up to the highest bet..
    int min_bet = get_highest_bet() - bets[player_index];
    // ... unless they can't afford that, in which case they go all in
    return std::clamp(min_bet, 0, balances[player_index]);
}

int PublicState::get_highest_bet() const {
    return std::max_element(bets.begin(), bets.end())[0];
}
