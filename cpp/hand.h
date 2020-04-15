#ifndef __HAND__
#define __HAND__

#include "card.h"
int strength_high_cards(t_card cards);
int strength_pair(t_card cards);
int strength_two_pairs(t_card cards);

int strength_royal_flush(t_card cards);     //strength range [0, 0]
int strength_straight_flush(t_card cards);  //strength range [1, 10]
int strength_four_of_a_kind(t_card cards);  //strength range [0, 168]
int strength_full_house(t_card cards);      //strength range [1, 167]
int strength_flush(t_card cards);           //strength range [0, 10]

#endif
