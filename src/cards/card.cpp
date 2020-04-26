#include "card.h"

#include <iostream>

static std::string SUITS[] = {"S", "H", "D", "C"};
static std::string RANKS[] = {"A", "K", "Q", "J", "10", "9", "8", "7", "6", "5",
    "4", "3", "2"};


Card::Card(t_card c) {
    int l = 63 - __builtin_clzll(c);
    suit = l % 4;
    rank = l / 4;
}

Card::Card(const std::string &s) {
    for (suit = 0; suit < 4; suit ++)
        if (SUITS[suit] == s.substr(0, 1)) break;
    for (rank = 0; rank < 13; rank ++)
        if (RANKS[rank] == s.substr(1)) break;
}

std::string Card::str() const {
    return SUITS[suit] + RANKS[rank];
}

t_card Card::encode() const {
    return 1ll << (suit + rank * 4);
}

bool operator==(const Card &c1, const Card &c2) {
    return c1.rank == c2.rank;
}

bool operator<(const Card &c1, const Card &c2) {
    return c1.rank > c2.rank;
}

std::vector<Card> decode_cards(t_card cards) {
    std::vector<Card> col;
    while(cards) {
        t_card c = lowbit(cards);
        col.push_back(Card(c));
        cards -= c;
    }
    return col;
}

t_card encode_cards(const std::vector <Card> &v) {
    t_card c = 0;
    for (size_t i = 0; i < v.size(); i ++)
        c |= v[i].encode();
    return c;
}

std::vector <Card> overlap_cards(const t_card &c1, const t_card &c2) {
    return decode_cards(c1 & c2);
}

size_t count_cards(t_card c) {
    size_t count = 0;
    while(c) count ++, c -= lowbit(c);
    return count;
}

void print_cards(t_card c) {
    std::vector <Card> cards = decode_cards(c);
    for (size_t i = 0; i < cards.size(); i ++) 
        std::cout << cards[i].str() << " ";
    std::cout << std::endl;
}
