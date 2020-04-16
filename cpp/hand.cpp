#include "hand.h"

#include <algorithm>
#include <string>
#include <vector>

static int strength_high_cards(t_card cards, int count) {
    int base = 1;
    for (int i = 1; i < count; i ++)
        base *= 13;
    int strength = 0;
    for (int i = 0; i < count; i ++) {
        card high_card = card(lowbit(cards));
        strength += high_card.rank * base;
        base /= 13;
        cards -= lowbit(cards);
    }
    return strength;
}

int strength_high_cards(t_card cards) {
    return strength_high_cards(cards, 5);
}

int strength_pair(t_card cards) {
    for (int i = 0; i < 13; i ++) {
        t_card pattern = 15ll << (i * 4);
        if (count_cards(cards & pattern)  == 2) {
            t_card remaining = cards & (~pattern);
            return i * (13 * 13 * 13) + strength_high_cards(remaining, 3);
        }
    }
    return -1;
}

int strength_two_pairs(t_card cards) {
    for (int i = 0; i < 13; i ++) {
        t_card pattern = 15ll << (i * 4);
        if (count_cards(cards & pattern)  == 2) {
            t_card remaining = cards & (~pattern);
            for (int j = i + 1; j < 13; j ++) {
                t_card pattern2 = 15ll << (j * 4);
                if (count_cards(remaining & pattern2)  == 2) 
                    return i * 13 * 13 + j * 13 + 
                        strength_high_cards(remaining & (~pattern2), 1);
            }
        }
    }
    return -1;
}

int strength_three_of_a_kind(t_card cards) {
    for (int i = 0; i < 13; i ++) {
        t_card pattern = 15ll << (i * 4);
        if (count_cards(cards & pattern)  == 3) {
            t_card remaining = cards & (~pattern);
            return i * (13 * 13) + strength_high_cards(remaining, 2);
        }
    }
    return -1;
}

int strength_straight(t_card cards) {

    for (int i = 0; i < 10; i ++) {
        bool found = true;
        for (int j = 0; j < 5; j ++) {
            int offset = (i + j) % 13;
            int section = 15 & (cards >> (offset * 4));
            if (section == 0) {
                found = 0;
                break;
            }
        }
        if (found) return i;
    }
    return -1;
}

int strength_flush(t_card cards) {
    t_card pattern = 0;
    for (int i = 0; i < 13; i ++)
        pattern += (1ll << (i * 4));
    for (int i = 0; i < 4; i ++) {
        t_card overlap = cards & (pattern << i);
        if (count_cards(overlap) >= 5)
            return strength_high_cards(overlap, 5);
    }
    return -1;
}

int strength_full_house(t_card cards) {
    for (int i = 0; i < 13; i ++) {
        t_card pattern = 15ll << (i * 4);
        if (count_cards(cards & pattern)  == 3) {
            t_card remaining = cards & (~pattern);
            for (int j = 0; j < 13; j ++) {
                t_card pattern2 = 15ll << (j * 4);
                if (count_cards(remaining & pattern2)  == 2) 
                    return i * 13 + j;
            }
        }
    }
    return -1;
}

int strength_four_of_a_kind(t_card cards) {
    for (int i = 0; i < 13; i ++) {
        t_card pattern = 15ll << (i * 4);
        if (count_cards(cards & pattern)  == 4) {
            t_card remaining = cards & (~pattern);
            return i * 13 + strength_high_cards(remaining, 1);
        }
    }
    return -1;
}

int strength_straight_flush(t_card cards) {
    if (strength_flush(cards) >= 0) return strength_straight(cards);
    return -1;
}

int strength_royal_flush(t_card cards) {
    if (strength_straight_flush(cards) == 0) return 0;
    return -1;
}

int hand_strength(t_card cards) {
    int base = 13 * 13 * 13 * 13 * 13;
    int s = -1;
    
    s = strength_straight_flush(cards);
    if (s >= 0) return s + base * 0;

    s = strength_four_of_a_kind(cards);
    if (s >= 0) return s + base * 1;

    s = strength_full_house(cards);
    if (s >= 0) return s + base * 2;

    s = strength_flush(cards);
    if (s >= 0) return s + base * 3;

    s = strength_straight(cards);
    if (s >= 0) return s + base * 4;

    s = strength_three_of_a_kind(cards);
    if (s >= 0) return s + base * 5;

    s = strength_two_pairs(cards);
    if (s >= 0) return s + base * 6;

    s = strength_pair(cards);
    if (s >= 0) return s + base * 7;

    s = strength_high_cards(cards);
    return s + base * 8;
}

std::vector <t_card> all_hands() {
    std::vector <t_card> hands;
    for (int a = 0; a < 52; a ++)
        for (int b = a + 1; b < 52; b ++)
            for (int c = b + 1; c < 52; c ++)
                for (int d = c + 1; d < 52; d ++)
                    for (int e = d + 1; e < 52; e ++) {
                        t_card hand = (1ll << a) + (1ll << b) + (1ll << c) + (1ll << d) + (1ll << e);
                        hands.push_back(hand);
                    }
    return hands;
}

std::vector <t_card> possible_hands(t_card usable, t_card unusable, 
        size_t max_extra, const std::vector <t_card> &searchlist) {
    std::vector <t_card> hands;
    for (size_t i = 0; i < searchlist.size(); i ++) 
        if ((searchlist[i] & unusable) == 0) {
            t_card overlap = usable & searchlist[i];
            size_t extra = count_cards(searchlist[i]) - count_cards(overlap);
            if (extra <= max_extra) hands.push_back(searchlist[i]);
        }
    return hands;
}

std::map <t_card, unsigned int> hand_to_rank(const std::vector <t_card> &hands) {
    std::vector <std::pair <int, t_card> > strengths;
    for (size_t i = 0; i < hands.size(); i ++)
        strengths.push_back(std::make_pair(hand_strength(hands[i]), hands[i]));
    std::sort(strengths.begin(), strengths.end());

    std::map <t_card, unsigned int> ranks;
    unsigned int rank = 0;
    int prev = strengths[0].first;
    for (size_t i = 0; i < strengths.size(); i ++) {
        if (prev < strengths[i].first) rank = i;
        ranks[strengths[i].second] = rank;
        prev = strengths[i].first;
    }
    return ranks;
}

static unsigned C(unsigned n, unsigned k) {
    if (k > n) return 0;
    if (k * 2 > n) k = n-k;
    if (k == 0) return 1;

    int result = n;
    for( int i = 2; i <= k; i++ ) {
        result *= (n-i+1);
        result /= i;
    }
    return result;
}

std::map <t_card, double> P_hand(const std::vector <t_card> &targets,
        t_card hand, size_t decksize, size_t tries) {
    unsigned t = C(decksize, tries);
    std::map <t_card, double> probs;
    for (size_t i = 0; i < targets.size(); i ++) {
        size_t missing = count_cards(targets[i]) - count_cards(hand & targets[i]);
        unsigned c = C(decksize-missing, tries-missing);
        probs[targets[i]] = double(c) / t;
    }
    return probs;
}

std::map <int, double> P_best_strength(const std::map <t_card, double> &p_hand) {
    std::map <int, double> p_strength;

    for (auto it = p_hand.begin(); it != p_hand.end(); it ++) {
        int strength = hand_strength(it->first);
        auto p = p_strength.find(strength);
        if (p == p_strength.end()) {
            p_strength[strength] = 0;
            p = p_strength.find(strength);
        }
        p->second += it->second;
    }

    double prev = 1;
    for (auto it = p_strength.begin(); it != p_strength.end(); it ++) {
        double t = it->second;
        it->second *= prev;
        prev *= (1 - t);
    }
    
    return p_strength;
}

#include <iostream>
void eval_hand(t_card pocket, t_card pub, eval *ret) {
    size_t decksize = 52 - count_cards(pocket) - count_cards(pub);
    size_t tries = 5 - count_cards(pub);

    ret->my_targets = possible_hands(pocket | pub, 0, tries, ret->my_targets);
    auto my_p_hand = P_hand(ret->my_targets, pocket | pub, decksize, tries);
    auto my_p_best = P_best_strength(my_p_hand);

    ret->op_targets = possible_hands(pub, pocket, tries + 2, ret->op_targets);
    auto op_p_hand = P_hand(ret->op_targets, pub, decksize, tries + 2);
    auto op_p_best = P_best_strength(op_p_hand);

    for (auto i = op_p_best.rbegin(); std::next(i, 1) != op_p_best.rend(); i ++) 
        std::next(i, 1)->second += i->second;
    std::cerr << op_p_best.begin()->second << std::endl;

    ret->win = ret->lose = ret->tie = 0;
    auto opit = op_p_best.begin();
    for (auto myit = my_p_best.begin(); myit != my_p_best.end(); myit ++) {
        while (opit->first < myit->first && opit != op_p_best.end())
            opit ++;
        if (opit == op_p_best.end()) {
            ret->win += myit->second;
            continue;
        }

        int my_strength = myit->first;
        int op_strength = opit->first;
        double my_prob = myit->second;
        double op_prob = opit->second;

        if (my_strength == op_strength) {       //could tie
            auto nextop = std::next(opit, 1);
            if (nextop != op_p_best.end()) {
                ret->win += my_prob * nextop->second;
                ret->tie += my_prob * (op_prob - nextop->second);
            } 
            else ret->tie += my_prob * op_prob;
            ret->lose += my_prob * (1 - op_prob);
            continue;
        }
        ret->win += my_prob * op_prob;
        ret->lose += my_prob * (1 - op_prob);
    }
}
