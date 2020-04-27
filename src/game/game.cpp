#include "game.h"
#include "cards/deck.h"

#include <iostream>


// private

void betting_actions(std::vector<Player>& players, PublicState& public_state) {
    // TODO implement
}

/// 1st betting round.
void pre_flop(std::vector<Player>& players, PublicState& public_state) {
    std::cout << std::endl
              << "## Pre-Flop" << std::endl
              << "############" << std::endl << std::endl;

    betting_actions(players, public_state);
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

    betting_actions(players, public_state);
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
    betting_actions(players, public_state);
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

    // print balances
    std::cout << "Balances: ";
    for (int balance: public_state.get_balances()) {
        std::cout << balance << ", ";
    }
    std::cout << std::endl;

    // deduct blinds
    int small_blind = public_state.bet(public_state.get_small_blind_index(), public_state.get_small_blind());
    int big_blind = public_state.bet(public_state.get_big_blind_index(), public_state.get_big_blind());
    std::cout << "Small blind: player " << public_state.get_small_blind_index() << " bets " << small_blind << std::endl
              << "Big blind:   player " << public_state.get_big_blind_index() << " bets " << big_blind << std::endl;

    // deal 2 cards to each player
    Deck deck = Deck();
    for (Player player : players) {
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
