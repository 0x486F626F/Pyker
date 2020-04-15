#include "hand.h"
#include <iostream>

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
    return 0;
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

int strength_royal_flush(t_card cards) {
    if (strength_straight_flush(cards) == 0) return 0;
    return -1;
}

int strength_straight_flush(t_card cards) {
    for (int s = 0; s < 10; s ++) { // highest card from A, K, ..., 4  
        t_card pattern = 0;
        for (int i = 0; i < 5; i ++) 
            pattern += 1ll << ((s + i) % 13 * 4);
        print_cards(pattern);
        for (int i = 0; i < 4; i ++) 
            if (count_cards((pattern << i) & cards) == 5) return s;
    }
    return -1;
}

int strength_four_of_a_kind(t_card cards) {
    t_card pattern = 1 + 2 + 4 + 8;
    for (int i = 0; i < 13; i ++) {
        t_card overlap = cards & (pattern << (i * 4)); 
        if (count_cards(overlap) == 4) {
            t_card high_cards = cards ^ pattern;
            card high_card = card(lowbit(high_cards));
            return i * 13 + high_card.rank;
        }
    }
    return -1;
}

int strength_flush(t_card cards) {
    for (int i = 0; i < 10; i ++) {
        uint8_t found = 1;
        for (int j = 0; j < 5; j ++)
            found *= 15 & (cards >> ((i+j) % 13 * 4));
        if (found) return i;
    }
    return -1;
}
