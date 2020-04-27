#include "player.h"

#include <iostream>
#include <tuple>

using namespace std;


Player::Player(PlayerController* controller, size_t player_index) {
    this->controller = controller;
    this->player_index = player_index;
    controller->set_player_index(player_index);
}

void Player::deal(t_card card) {
    hand = hand & card;
}

int Player::do_betting_action(const PublicState& public_state) {
    print_hand();
    int bet = controller->bet(hand, public_state);

    if (bet == FOLD) {
        std::cout << "Player " << player_index << " folds." << std::endl;
    } else {
        // check whether the bet is in the valid range
        if (bet < public_state.get_min_bet(player_index)) {
            throw domain_error("Player " + to_string(player_index) + " attempted to bet " + to_string(bet) +
                               ", but must bet at least bet " + to_string(public_state.get_min_bet(player_index)));
        }
        if (bet > public_state.get_balances()[player_index]) {
            throw domain_error("Player " + to_string(player_index) + " attempted to bet " + to_string(bet) +
                               ", but only has " + to_string(public_state.get_balances()[player_index]));
        }

        std::cout << "Player " << player_index << " bets " << bet << "." << std::endl;
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
