#include "cli_controller.h"

#include <tuple>
#include <iostream>


int CliController::bet(std::vector<Card> hand, const PublicState& public_state, BetRange bet_range) {
    auto [min_bet, max_bet] = bet_range;

    int bet;
    bool bet_ok = false;

    std::cout << "Type your bet. Type \"-1\" to fold." << std::endl;

    while (!bet_ok) {
        std::cin >> bet;

        if (bet < min_bet && bet != FOLD) {
            std::cout << "Bet must be at least " << min_bet << "." << std::endl;
        } else if (bet > max_bet) {
            std::cout << "Bet must be at most " << max_bet << "." << std::endl;
        } else {
            bet_ok = true;
        }
    }

    return bet;
}
