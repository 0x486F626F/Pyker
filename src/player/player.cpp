#include "player.h"

#include <iostream>
#include <tuple>


Player::Player(PlayerController* controller, size_t player_index) {
    this->controller = controller;
    this->player_index = player_index;
    controller->set_player_index(player_index);
}

void Player::deal(t_card card) {
    hand = hand & card;
}

int Player::do_betting_action(const PublicState& public_state, BetRange bet_range) {
    print_hand();
    int bet = controller->bet(hand, public_state, bet_range);

    if (bet == FOLD) {
        std::cout << "Player " << player_index << " folds." << std::endl;
    } else {
        std::cout << "Player " << player_index << " bets " << bet << "." << std::endl;

        // players cannot bet more than they have
        if (bet > public_state.get_balances()[player_index]) {
            throw std::domain_error(
                "Player " + std::to_string(player_index) + " attempted to bet " + std::to_string(bet) +
                ", but only has " + std::to_string(public_state.get_balances()[player_index])
            );
        }
    }

    return bet;
}

void Player::reset() {
    hand = 0;
}

void Player::print_hand() const {
    std::cout << "Player " << player_index << "'s hand: ";
    print_cards(hand);
}
