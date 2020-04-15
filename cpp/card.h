#ifndef __CARD__
#define __CARD__

#define lowbit(x) x & -x

#include <cstdint>
#include <string>
#include <vector>

// The type card holds the information of one or more cards using lowest 52 bits
typedef uint64_t t_card;

// The class the interprets a encoded card into human-readable form
struct card {
    uint8_t suit;   //Spades = 0, Hearts = 1, Diamonds = 2 and Clubs = 3
    uint8_t rank;   //A=0, K=1, Q=2, J=3, 10=4, ..., 2=12
    card(t_card c);
    card(const std::string &s);
    std::string str() const;
    t_card encode() const;
};

// Take in a t_card, return a list of card classes
std::vector <card> decode_cards(t_card cards);
t_card encode_cards(const std::vector <card> &v);

std::vector <card> overlap_cards(const t_card &c1, const t_card &c2);
size_t count_cards(t_card c);
void print_cards(t_card c);

#endif
