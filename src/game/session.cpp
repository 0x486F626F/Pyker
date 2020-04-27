#include "session.h"
#include "game.h"

#include <iostream>
#include <utility>


Session::Session(std::vector<Player> players, int starting_balance, int big_blind, int small_blind) {
    this->players = std::move(players);
    public_state = PublicState {
        .big_blind = big_blind,
        .small_blind = small_blind,
        .balances = std::vector(this->players.size(), starting_balance),
        .dealer_index = 0,  // TODO choose dealer randomly
    };
}

void Session::start() {
    // the game ends if there are less than two players left
    // TODO use public_state.remaining_player_indices().size() instead
    while (players.size() >= 2) {
        std::cout << std::endl << std::endl << std::endl
                  << "##############" << std::endl
                  << "## New Hand ##" << std::endl
                  << "##############" << std::endl
                  << std::endl << std::endl;
        play_hand(players, public_state);
    }
}
