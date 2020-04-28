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

int Player::do_betting_action(const PublicState& public_state) {
    print_hand();
    return controller->bet(hand, public_state);
}

void Player::reset() {
    hand = 0;
}

void Player::print_hand() const {
    std::cout << "Player " << player_index << "'s hand: ";
    print_cards(hand);
}
