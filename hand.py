from typing import List, Optional

from deck import Card, NUM_RANKS, SUITS


class Hand:
    cards: List[Card]
    hand: Optional[List]  # TODO make this a Tuple[int, List[int], List[Card]]

    def __init__(self, cards):
        self.cards = cards
        self.hand = None
        self.hand = self._royal_flush()
        self.hand = self._straight_flush()
        self.hand = self._four_of_a_kind()
        self.hand = self._full_house()
        self.hand = self._flush()
        self.hand = self._straight()
        self.hand = self._three_of_a_kind()
        self.hand = self._two_pairs()
        self.hand = self._pair()
        self.hand = self._high_card()

    def _royal_flush(self):
        if self.hand is not None:
            return self.hand
        for suit in SUITS:
            pattern = [Card(suit, (i + 9) % NUM_RANKS) for i in range(5)]
            found = True
            for each in pattern:
                if each not in self.cards:
                    found = False
            if found:
                return [9, [0], pattern]
        return None

    def _straight_flush(self):
        if self.hand is not None:
            return self.hand
        for i in range(9):
            for suit in SUITS:
                pattern = [Card(suit, (8 - i + j)) for j in range(5)]
                found = True
                for each in pattern:
                    if each not in self.cards:
                        found = False
                if found:
                    return [8, [8 - i], pattern]
        return None

    def _four_of_a_kind(self):
        if self.hand is not None:
            return self.hand
        for i in range(NUM_RANKS):
            pattern = [Card(suit, (NUM_RANKS - i) % NUM_RANKS) for suit in SUITS]
            found = True
            for each in pattern:
                if each not in self.cards:
                    found = False
            if found:
                a = self._add_highest(self.cards, pattern)
                return [7, [NUM_RANKS - i, a], pattern]

    def _full_house(self):
        if self.hand is not None:
            return self.hand
        for i in range(NUM_RANKS):
            for j in range(NUM_RANKS):
                if i != j:
                    t = []
                    d = []
                    for card in self.cards:
                        if card.rank == (NUM_RANKS - i) % NUM_RANKS:
                            t.append(card)
                        if card.rank == (NUM_RANKS - j) % NUM_RANKS:
                            d.append(card)
                    if len(t) == 3 and len(d) == 2:
                        return [6, [NUM_RANKS - i, NUM_RANKS - j], t + d]
        return None

    def _flush(self):
        if self.hand is not None:
            return self.hand
        for suit in SUITS:
            flush_cards = [card for card in self.cards if card.suit == suit]
            if len(flush_cards) >= 5:
                pattern = []
                return [5, [self._add_highest(flush_cards, pattern) for _ in range(5)], pattern]
        return None

    def _straight(self):
        if self.hand is not None:
            return self.hand
        for i in range(10):
            pattern = []
            for j in range(5):
                for card in self.cards:
                    if card.rank == (9 - i + j) % NUM_RANKS:
                        pattern.append(card)
                        break
                if len(pattern) <= j:
                    break
            if len(pattern) == 5:
                return [4, [9 - i], pattern]
        return None

    def _three_of_a_kind(self):
        if self.hand is not None:
            return self.hand
        for i in range(NUM_RANKS):
            pattern = [card for card in self.cards if card.rank == (NUM_RANKS - i) % NUM_RANKS]
            if len(pattern) == 3:
                a = self._add_highest(self.cards, pattern)
                b = self._add_highest(self.cards, pattern)
                return [3, [NUM_RANKS - i, a, b], pattern]
        return None

    def _two_pairs(self):
        if self.hand is not None:
            return self.hand
        for i in range(NUM_RANKS):
            for j in range(i+1, NUM_RANKS):
                t = []
                d = []
                for card in self.cards:
                    if card.rank == (NUM_RANKS - i) % NUM_RANKS:
                        t.append(card)
                    if card.rank == (NUM_RANKS - j) % NUM_RANKS:
                        d.append(card)
                if len(t) == 2 and len(d) == 2:
                    pattern = t + d
                    a = self._add_highest(self.cards, pattern)
                    return [2, [NUM_RANKS - i, NUM_RANKS - j, a], pattern]
        return None

    def _pair(self):
        if self.hand is not None:
            return self.hand
        for i in range(NUM_RANKS):
            pattern = [card for card in self.cards if card.rank == (NUM_RANKS - i) % NUM_RANKS]
            if len(pattern) == 2:
                a = self._add_highest(self.cards, pattern)
                b = self._add_highest(self.cards, pattern)
                c = self._add_highest(self.cards, pattern)
                return [1, [NUM_RANKS - i, a, b, c], pattern]
        return None

    def _high_card(self):
        if self.hand is not None:
            return self.hand
        pattern = []
        for _ in range(min(5, len(self.cards))):
            self._add_highest(self.cards, pattern)
        return [0, [e[1] for e in pattern], pattern]

    def _add_highest(self, cards: List[Card], pattern: List[Card]) -> int:
        highest_card = None

        for card in cards:
            if card not in pattern and (
                highest_card is None or
                # use (rank - 1) % NUM_RANKS to make sure the Ace is the highest card
                (card.rank - 1) % NUM_RANKS > (highest_card.rank - 1) % NUM_RANKS
            ):
                highest_card = card

        pattern.append(highest_card)
        return (highest_card.rank - 1) % NUM_RANKS + 1

    def __lt__(self, other):
        if self.hand[0] != other.hand[0]:
            return self.hand[0] < other.hand[0]
        for i in range(len(self.hand[1])):
            if self.hand[1][i] != other.hand[1][i]:
                return self.hand[1][i] < other.hand[1][i]
        return False

    def __eq__(self, other):
        if self.hand[0] != other.hand[0]:
            return False
        for i in range(len(self.hand[1])):
            if self.hand[1][i] != other.hand[1][i]:
                return False
        return True

    def print_hand(self):
        print(self.hand[0], self.hand[1])
        print(' '.join([str(c) for c in self.hand[2]]))
