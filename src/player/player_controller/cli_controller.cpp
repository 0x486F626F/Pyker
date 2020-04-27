#include "cli_controller.h"

#include <iostream>


int CliController::bet(t_card hand, const PublicState& public_state) {
    int min_bet = public_state.get_min_bet(player_index);
    int max_bet = public_state.get_balances()[player_index];

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
