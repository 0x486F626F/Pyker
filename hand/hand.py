from typing import List, Optional

from deck import Card
from ._hand_checks import HAND_CHECKS, HandInfo


class Hand:
    cards: List[Card]
    hand: Optional[HandInfo] = None

    def __init__(self, cards):
        self.cards = cards

        for check in HAND_CHECKS:
            result = check(cards)
            if result is not None:
                self.hand = result
                break

    def __lt__(self, other) -> bool:
        if self.hand[0] != other.hand[0]:
            return self.hand[0] < other.hand[0]
        for i in range(len(self.hand[1])):
            if self.hand[1][i] != other.hand[1][i]:
                return self.hand[1][i] < other.hand[1][i]
        return False

    def __eq__(self, other) -> bool:
        if self.hand[0] != other.hand[0]:
            return False
        for i in range(len(self.hand[1])):
            if self.hand[1][i] != other.hand[1][i]:
                return False
        return True

    def print_hand(self):
        print(self.hand[0], self.hand[1])
        print(' '.join([str(c) for c in self.hand[2]]))
