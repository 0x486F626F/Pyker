#include "player.h"

#include <iostream>
#include <tuple>


Player::Player(PlayerController* controller, int initial_balance) {
    this->controller = controller;
    balance = initial_balance;
}

void Player::deal(Card card) {
    hand.push_back(card);
}

int Player::do_betting_action(BetRange bet_range) {
    print_hand();
    int bet = controller->bet(hand, bet_range);

    if (bet == FOLD) {
        std::cout << "Fold." << std::endl;
        folded = true;
    } else {
        std::cout << "Bet: " << bet << std::endl;
        // TODO make sure PlayerControllers can't choose bet > balance
        balance -= bet;
    }

    return bet;
}

void Player::reset() {
    folded = false;
    hand.clear();
}

void Player::reduce_balance(int amount) {
    balance -= amount;
}

void Player::set_balance(int b) {
    this->balance = b;
}

bool Player::has_folded() const {
    return folded;
}

void Player::print_hand() const {
    for (Card card : hand) {
        std::cout << "Hand: " << card.str() << std::endl;
    }
}
