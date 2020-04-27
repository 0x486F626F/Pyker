#include "public_state.h"


template <typename T>
/// @return the elements of vec at the specified indices
std::vector<T> vector_subset(const std::vector<T>& vec, const std::vector<size_t>& indices) {
    std::vector<T> subset;
    for (size_t index : indices) {
        subset.push_back(vec[index]);
    }
    return subset;
}


void PublicState::start_game() {
    bets = std::vector(balances.size(), 0);
    folded = std::vector(balances.size(), false);
    community_cards.clear();
    // TODO move dealer_index
}

std::vector<size_t> PublicState::remaining_player_indices() const {
    std::vector<size_t> indices;

    for (int i = 0; i < balances.size(); i++) {
        if (balances[i] > 0 && bets[i] > 0) {
            indices.push_back(i);
        }
    }

    return indices;
}

std::vector<int> PublicState::remaining_balances() const {
    return vector_subset(balances, remaining_player_indices());
}

std::vector<int> PublicState::remaining_bets() const {
    return vector_subset(bets, remaining_player_indices());
}

std::vector<bool> PublicState::remaining_folded() const {
    return vector_subset(folded, remaining_player_indices());
}
