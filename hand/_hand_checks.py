from typing import List, Optional, Tuple

from deck import Card, NUM_RANKS, SUITS


HandInfo = Tuple[int, List[int], List[Card]]


def royal_flush(cards: List[Card]) -> Optional[HandInfo]:
    for suit in SUITS:
        pattern = [Card(suit, (i + 9) % NUM_RANKS) for i in range(5)]
        found = True
        for card in pattern:
            if card not in cards:
                found = False
        if found:
            return 9, [0], pattern
    return None


def straight_flush(cards: List[Card]) -> Optional[HandInfo]:
    for i in range(9):
        for suit in SUITS:
            pattern = [Card(suit, (8 - i + j)) for j in range(5)]
            found = True
            for card in pattern:
                if card not in cards:
                    found = False
            if found:
                return 8, [8 - i], pattern
    return None


def four_of_a_kind(cards: List[Card]) -> Optional[HandInfo]:
    for i in range(NUM_RANKS):
        pattern = [Card(suit, (NUM_RANKS - i) % NUM_RANKS) for suit in SUITS]
        found = True
        for card in pattern:
            if card not in cards:
                found = False
        if found:
            a = _add_highest(cards, pattern)
            return 7, [NUM_RANKS - i, a], pattern


def full_house(cards: List[Card]) -> Optional[HandInfo]:
    for i in range(NUM_RANKS):
        for j in range(NUM_RANKS):
            if i != j:
                t = []
                d = []
                for card in cards:
                    if card.rank == (NUM_RANKS - i) % NUM_RANKS:
                        t.append(card)
                    if card.rank == (NUM_RANKS - j) % NUM_RANKS:
                        d.append(card)
                if len(t) == 3 and len(d) == 2:
                    return 6, [NUM_RANKS - i, NUM_RANKS - j], t + d
    return None


def flush(cards: List[Card]) -> Optional[HandInfo]:
    for suit in SUITS:
        flush_cards = [card for card in cards if card.suit == suit]
        if len(flush_cards) >= 5:
            pattern = []
            return 5, [_add_highest(flush_cards, pattern) for _ in range(5)], pattern
    return None


def straight(cards: List[Card]) -> Optional[HandInfo]:
    for i in range(10):
        pattern = []
        for j in range(5):
            for card in cards:
                if card.rank == (9 - i + j) % NUM_RANKS:
                    pattern.append(card)
                    break
            if len(pattern) <= j:
                break
        if len(pattern) == 5:
            return 4, [9 - i], pattern
    return None


def three_of_a_kind(cards: List[Card]) -> Optional[HandInfo]:
    for i in range(NUM_RANKS):
        pattern = [card for card in cards if card.rank == (NUM_RANKS - i) % NUM_RANKS]
        if len(pattern) == 3:
            a = _add_highest(cards, pattern)
            b = _add_highest(cards, pattern)
            return 3, [NUM_RANKS - i, a, b], pattern
    return None


def two_pairs(cards: List[Card]) -> Optional[HandInfo]:
    for i in range(NUM_RANKS):
        for j in range(i+1, NUM_RANKS):
            t = []
            d = []
            for card in cards:
                if card.rank == (NUM_RANKS - i) % NUM_RANKS:
                    t.append(card)
                if card.rank == (NUM_RANKS - j) % NUM_RANKS:
                    d.append(card)
            if len(t) == 2 and len(d) == 2:
                pattern = t + d
                a = _add_highest(cards, pattern)
                return 2, [NUM_RANKS - i, NUM_RANKS - j, a], pattern
    return None


def pair(cards: List[Card]) -> Optional[HandInfo]:
    for i in range(NUM_RANKS):
        pattern = [card for card in cards if card.rank == (NUM_RANKS - i) % NUM_RANKS]
        if len(pattern) == 2:
            a, b, c = 0, 0, 0
            if len(cards) >= 3:
                a = _add_highest(cards, pattern)
            if len(cards) >= 4:
                b = _add_highest(cards, pattern)
            if len(cards) >= 5:
                c = _add_highest(cards, pattern)
            return 1, [NUM_RANKS - i, a, b, c], pattern
    return None


def high_card(cards: List[Card]) -> Optional[HandInfo]:
    pattern = []
    for _ in range(min(5, len(cards))):
        _add_highest(cards, pattern)
    return 0, [e.rank for e in pattern], pattern


def _add_highest(cards: List[Card], pattern: List[Card]) -> int:
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


# A list of checks to determine which combination a hand holds,
# ordered from best to worst combination
HAND_CHECKS = [
    royal_flush, straight_flush, four_of_a_kind, full_house, flush,
    straight, three_of_a_kind, two_pairs, pair, high_card,
]
