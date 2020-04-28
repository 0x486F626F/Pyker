#include "game.h"
#include "cards/deck.h"

#include <iostream>

using namespace std;


// private

void print_status(const PublicState& public_state) {
    // balances
    std::cout << "Balances: ";
    for (int balance: public_state.get_balances()) {
        std::cout << balance << ", ";
    }
    std::cout << std::endl;

    // bets
    if (public_state.get_highest_bet() > 0) {
        std::cout << "Bets: ";
        for (int bet: public_state.get_bets()) {
            std::cout << bet << ", ";
        }
        std::cout << std::endl;
    }

    // folded players
    if (public_state.get_folded_player_indices().size() > 0) {
        std::cout << "Folded: Player(s) ";
        for (size_t player_index: public_state.get_folded_player_indices()) {
            std::cout << player_index << ", ";
        }
        std::cout << std::endl;
    }
}

/// Checks whether the amount of chips a player wants to bet is legal.
void check_bet(int bet, PublicState& public_state, size_t player_index) {
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
}

/// Lets the player take turns betting, until all players agree on a bet (or fold).
void do_betting_actions(std::vector<Player>& players, PublicState& public_state, size_t starting_player_index) {
    print_status(public_state);
    std::cout << std::endl;

    size_t current_player_index = starting_player_index;
    // if the current player has folded, go to the next player who hasn't folded and isn't bankrupt
    while (public_state.get_folded()[current_player_index]) {
        current_player_index = public_state.next_player_after(current_player_index);
    }

    // indexes of players who agree with the highest bet or have folded.
    // players agree with the highest bet if they have raised to that bet (not counting blinds) or called.
    std::vector<size_t> done;

    while (done.size() < public_state.get_remaining_player_indices().size()) {
        int bet = players[current_player_index].do_betting_action(public_state);
        check_bet(bet, public_state, current_player_index);

        if (bet == FOLD) {
            public_state.fold(current_player_index);
        } else {
            // check whether this bet is a raise
            if (bet + public_state.get_bets()[current_player_index] > public_state.get_highest_bet()) {
                // all players can bet again (except for those who folded) => need to reset done
                done = public_state.get_folded_player_indices();
            }
            public_state.bet(current_player_index, bet);
        }
        done.push_back(current_player_index);

        // go to the next player who hasn't folded and isn't bankrupt
        do {
            current_player_index = public_state.next_player_after(current_player_index);
        } while (public_state.get_folded()[current_player_index]);
    }
}

/// 1st betting round.
void pre_flop(std::vector<Player>& players, PublicState& public_state) {
    std::cout << std::endl
              << "## Pre-Flop" << std::endl
              << "############" << std::endl << std::endl;

    public_state.start_round();
    // the player after the big blind starts betting
    do_betting_actions(players, public_state, public_state.next_player_after(public_state.get_big_blind_index()));
}

/// 2nd betting round.
void flop(std::vector<Player>& players, PublicState& public_state, Deck& deck) {
    std::cout << std::endl
              << "## Flop" << std::endl
              << "########" << std::endl << std::endl;

    // burn a card, then reveal 3 community cards
    deck.deal();
    public_state.reveal_community_card(deck.deal());
    public_state.reveal_community_card(deck.deal());
    public_state.reveal_community_card(deck.deal());

    public_state.start_round();
    do_betting_actions(players, public_state, public_state.get_small_blind_index());
}

/// 3rd and 4th betting rounds.
void turn_river(std::vector<Player>& players, PublicState& public_state, Deck& deck) {
    std::cout << std::endl
              << "## Turn/River" << std::endl
              << "##############" << std::endl << std::endl;

    // burn a card, then reveal a community card
    deck.deal();
    public_state.reveal_community_card(deck.deal());

    public_state.start_round();
    do_betting_actions(players, public_state, public_state.get_small_blind_index());
}

void showdown(std::vector<Player>& players, PublicState& public_state) {
    // TODO
    // TODO if more than one player didn't fold, reveal their cards (add them to the log?)

    // cleanup
    public_state.clear_bets();
}


// public

void play_hand(std::vector<Player>& players, PublicState& public_state) {
    public_state.start_game();
    print_status(public_state);
    std::cout << "Dealer: Player " << public_state.get_dealer_index() << std::endl;

    // deduct blinds
    int small_blind = public_state.bet(public_state.get_small_blind_index(), public_state.get_small_blind());
    int big_blind = public_state.bet(public_state.get_big_blind_index(), public_state.get_big_blind());
    std::cout << "Small blind: player " << public_state.get_small_blind_index() << " bets " << small_blind << std::endl
              << "Big blind:   player " << public_state.get_big_blind_index() << " bets " << big_blind << std::endl;

    // deal 2 cards to each player
    Deck deck = Deck();
    for (Player& player : players) {
        player.reset();
        player.deal(deck.deal());
        player.deal(deck.deal());
    }

    // start playing
    pre_flop(players, public_state);
    flop(players, public_state, deck);
    turn_river(players, public_state, deck);
    turn_river(players, public_state, deck);
    showdown(players, public_state);
}
