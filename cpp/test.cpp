#include "hand.h"

#include <string>
#include <iostream>

int main() {

    std::string a[] = {"SK", "HK", "D3", "C4", "S5"};
    std::vector <card> cards;
    for (int i = 0; i < 5; i ++)
        cards.push_back(card(a[i]));
    t_card encoded = encode_cards(cards);
    print_cards(encoded);
    std::cerr << strength_pair(encoded) << std::endl;

    return 0;
}
