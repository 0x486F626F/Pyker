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
    if (s >= 0) return s + base;

    s = strength_four_of_a_kind(cards);
    if (s >= 0) return s + base * 2;

    s = strength_full_house(cards);
    if (s >= 0) return s + base * 3;

    s = strength_flush(cards);
    if (s >= 0) return s + base * 4;

    s = strength_straight(cards);
    if (s >= 0) return s + base * 5;

    s = strength_three_of_a_kind(cards);
    if (s >= 0) return s + base * 6;

    s = strength_two_pairs(cards);
    if (s >= 0) return s + base * 7;

    s = strength_pair(cards);
    if (s >= 0) return s + base * 8;

    s = strength_high_cards(cards);
    return s + base * 9;
}
