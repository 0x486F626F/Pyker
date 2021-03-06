#include "kuhn_trainer.h"

#include <iostream>


void KuhnTrainer::train(int num_iterations) {
    Deck deck = {1, 2, 3};
    double utility = 0;

    for (int i = 0; i < num_iterations; i++) {
        shuffle(deck);
        utility += cfr(deck, "", 1, 1);
    }

    std::cout << "Average game value: " << (utility / num_iterations) << std::endl;
    // print nodes
    for (auto const& pair : node_map) {
        pair.second.print();
    }
}

void KuhnTrainer::shuffle(Deck& deck) {
    // shuffle cards using Fisher-Yates shuffle
    for (size_t card1_index = deck.size() - 1; card1_index > 0; card1_index--) {
        size_t card2_index = std::uniform_int_distribution<>(0, card1_index)(rng);
        Card tmp = deck[card1_index];
        deck[card1_index] = deck[card2_index];
        deck[card2_index] = tmp;
    }
}

double KuhnTrainer::cfr(Deck cards, std::string history, double p0, double p1) {
    int actions = history.length();
    int player = actions % 2;
    int opponent = 1 - player;

    // return payoff for terminal states
    if (actions > 1) {
        bool terminal_pass = history[actions - 1] == 'p';
        bool double_bet = history.substr(actions - 2, 2) == "bb";
        bool player_card_is_higher = cards[player] > cards[opponent];
        if (terminal_pass) {
            if (history == "pp") {
                return player_card_is_higher ? 1 : -1;
            } else {
                return 1;
            }
        } else if (double_bet) {
            return player_card_is_higher ? 2 : -2;
        }
    }

    InformationSet info_set = std::to_string(cards[player]) + history;
    auto node = get_node(info_set);

    // for each action, recursively call cfr with additional history and probability
    std::array<double, NUM_ACTIONS> strategy = node->second.get_strategy(player == 0 ? p0 : p1);
    std::array<double, NUM_ACTIONS> utilities{};
    double node_utility = 0;
    for (int action = 0; action < NUM_ACTIONS; action++) {
        std::string next_history = history + (action == 0 ? "p" : "b");
        utilities[action] = player == 0
            ? - cfr(cards, next_history, p0 * strategy[action], p1)
            : - cfr(cards, next_history, p0, p1 * strategy[action]);
        node_utility += strategy[action] * utilities[action];
    }

    // for each action, compute and accumulate counterfactual regret
    for (int action = 0; action < NUM_ACTIONS; action++) {
        double regret = utilities[action] - node_utility;
        node->second.regret_sum[action] += (player == 0 ? p1 : p0) * regret;
    }

    return node_utility;
}

std::map<InformationSet, Node>::iterator KuhnTrainer::get_node(const InformationSet &info_set) {
    // TODO make this a bit nicer, maybe use a better return type
    auto iter = node_map.find(info_set);
    if (iter == node_map.end()) {
        // node not found => create and insert it
        node_map[info_set] = Node(info_set);
        return node_map.find(info_set);
    } else {
        return iter;
    }
}
