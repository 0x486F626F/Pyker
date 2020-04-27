#include "session.h"
#include "game.h"

#include <iostream>
#include <utility>


Session::Session(std::vector<Player> players, int starting_balance, int small_blind, int big_blind)
    : public_state(players.size(), small_blind, big_blind, starting_balance)
{
    this->players = std::move(players);
}

void Session::start() {
    // the game ends if there are less than two players left
    while (public_state.get_remaining_player_indices().size() >= 2) {
        std::cout << std::endl << std::endl
                  << "##############" << std::endl
                  << "## New Hand ##" << std::endl
                  << "##############" << std::endl
                  << std::endl;
        play_hand(players, public_state);
    }
}
