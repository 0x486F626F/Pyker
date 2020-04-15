import random
from typing import List, Optional


NUM_RANKS = 13
NUM_SUITS = 4
RANKS = range(NUM_RANKS)
# TODO add RANKS_REVERSED
SUITS = range(NUM_SUITS)

# The unicode code point for the ace of each suit.
UNICODE_BASE_CODE_POINTS = [127137, 127153, 127169, 127185]


class Card:
    suit: int
    rank: int

    def __init__(self, suit: int, rank: int):
        """
        :param suit: The suit of the card, where Spades = 0, Hearts = 1, Diamonds = 2 and Clubs = 3.
        :param rank: The rank of the card, where Ace = 0, Two = 1, ..., Ten = 9, Jack = 10, Queen = 11 and King = 12.
        """
        assert(0 <= suit < NUM_SUITS)
        assert(0 <= rank < NUM_RANKS)
        self.suit = suit
        self.rank = rank

    def __str__(self):
        rank = self.rank if self.rank < 11 else self.rank + 1
        return chr(UNICODE_BASE_CODE_POINTS[self.suit] + rank)


class Deck:
    cards: List[Card]
    index: int

    def __init__(self):
        self.cards = [Card(suit, rank) for suit in SUITS for rank in RANKS]
        random.shuffle(self.cards)
        self.index = 0

    def deal(self) -> Optional[Card]:
        if self.index >= NUM_SUITS * NUM_RANKS:
            # the deck is empty
            return None

        self.index += 1
        return self.cards[self.index - 1]
