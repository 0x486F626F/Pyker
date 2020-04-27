#include "session.h"
#include "game.h"

#include <iostream>
#include <utility>


Session::Session(std::vector<Player> players, int starting_balance, int big_blind, int small_blind) {
    this->players = std::move(players);
    public_state = PublicState {
        .big_blind = big_blind,
        .small_blind = small_blind,
        // TODO initialise the other fields
    };
}

void Session::start() {
    // the game ends if there are less than two players left
    while (players.size() >= 2) {
        std::cout << std::endl
            << "##############" << std::endl
            << "## New Hand ##" << std::endl
            << "##############" << std::endl
            << std::endl;
        play_hand(players, public_state);
    }
}
