#ifndef __HAND__
#define __HAND__

#include "card.h"

#include <map>
#include <set>

int strength_high_cards(t_card cards);
int strength_pair(t_card cards);
int strength_two_pairs(t_card cards);
int strength_three_of_a_kind(t_card cards);  
int strength_straight(t_card cards);
int strength_flush(t_card cards);
int strength_full_house(t_card cards);  
int strength_four_of_a_kind(t_card cards);
int strength_straight_flush(t_card cards);
int strength_royal_flush(t_card cards);  
// compute the strength of a given hand, lower is better
int hand_strength(t_card cards);

std::vector <t_card> all_hands();
std::vector <t_card> possible_hands(t_card usable, t_card unusable, 
        size_t max_extra, const std::vector <t_card> &searchlist);
std::map <t_card, unsigned int> hand_to_rank(const std::vector <t_card> &hands);
std::map <t_card, double> P_hand(const std::vector <t_card> &hands,
        t_card hand, size_t decksize, size_t tries);
std::map <int, double> P_best_strength(const std::map <t_card, double> &p_hand);

struct eval {
    double win, tie, lose;
    std::vector <t_card> my_targets;
    std::vector <t_card> op_targets;
    eval() {
        my_targets = op_targets = all_hands();
    }
};
void eval_hand(t_card pocket, t_card pub, eval *ret);

#endif
