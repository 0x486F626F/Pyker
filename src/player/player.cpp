#include "player.h"

#include <iostream>
#include <tuple>


Player::Player(PlayerController* controller) {
    this->controller = controller;
}

void Player::deal(Card card) {
    hand.push_back(card);
}

int Player::do_betting_action(const PublicState& public_state, BetRange bet_range) {
    print_hand();
    int bet = controller->bet(hand, public_state, bet_range);

    if (bet == FOLD) {
        std::cout << "Fold." << std::endl;
    } else {
        std::cout << "Bet: " << bet << std::endl;
        // TODO make sure PlayerControllers can't choose bet > balance
    }

    return bet;
}

void Player::reset() {
    hand.clear();
}

void Player::print_hand() const {
    for (Card card : hand) {
        std::cout << "Hand: " << card.str() << std::endl;
    }
}
