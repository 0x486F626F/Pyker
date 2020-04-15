import deck


interpreter = deck.Deck()


class Hand(object):
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
        for i in range(4):
            pattern = [[i, (j+9) % 13] for j in range(5)]
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
            for p in range(4):
                pattern = [[p, (8-i+j)] for j in range(5)]
                found = True
                for each in pattern:
                    if each not in self.cards:
                        found = False
                if found:
                    return [8, [8-i], pattern]
        return None

    def _four_of_a_kind(self):
        if self.hand is not None:
            return self.hand
        for i in range(13):
            pattern = [[p, (13-i) % 13] for p in range(4)]
            found = True
            for each in pattern:
                if each not in self.cards:
                    found = False
            if found:
                a = self._add_highest(self.cards, pattern)
                return [7, [13-i, a], pattern]

    def _full_house(self):
        if self.hand is not None:
            return self.hand
        for i in range(13):
            for j in range(13):
                if i != j:
                    t = []
                    d = []
                    for each in self.cards:
                        if each[1] == (13-i) % 13:
                            t.append(each)
                        if each[1] == (13-j) % 13:
                            d.append(each)
                    if len(t) == 3 and len(d) == 2:
                        return [6, [13-i, 13-j], t+d]
        return None

    def _flush(self):
        if self.hand is not None:
            return self.hand
        for i in range(4):
            flushcards = []
            for each in self.cards:
                if each[0] == i:
                    flushcards.append(each)
            if len(flushcards) >= 5:
                pattern = []
                return [5, [self._add_highest(flushcards, pattern) for _ in range(5)], pattern]
        return None

    def _straight(self):
        if self.hand is not None:
            return self.hand
        for i in range(10):
            pattern = []
            for j in range(5):
                for each in self.cards:
                    if each[1] == (9-i+j) % 13:
                        pattern.append(each)
                        break
                if len(pattern) <= j:
                    break
            if len(pattern) == 5:
                return [4, [9-i], pattern]
        return None

    def _three_of_a_kind(self):
        if self.hand is not None:
            return self.hand
        for i in range(13):
            pattern = []
            for each in self.cards:
                if each[1] == (13-i) % 13:
                    pattern.append(each)
            if len(pattern) == 3:
                a = self._add_highest(self.cards, pattern)
                b = self._add_highest(self.cards, pattern)
                return [3, [13-i, a, b], pattern]
        return None

    def _two_pairs(self):
        if self.hand is not None:
            return self.hand
        for i in range(13):
            for j in range(i+1, 13):
                t = []
                d = []
                for each in self.cards:
                    if each[1] == (13-i) % 13:
                        t.append(each)
                    if each[1] == (13-j) % 13:
                        d.append(each)
                if len(t) == 2 and len(d) == 2:
                    pattern = t + d
                    a = self._add_highest(self.cards, pattern)
                    return [2, [13-i, 13-j, a], pattern]
        return None

    def _pair(self):
        if self.hand is not None:
            return self.hand
        for i in range(13):
            pattern = []
            for each in self.cards:
                if each[1] == (13-i) % 13:
                    pattern.append(each)
            if len(pattern) == 2:
                a = self._add_highest(self.cards, pattern)
                b = self._add_highest(self.cards, pattern)
                c = self._add_highest(self.cards, pattern)
                return [1, [13-i, a, b, c], pattern]
        return None

    def _high_card(self):
        if self.hand is not None:
            return self.hand
        pattern = []
        for i in range(5):
            self._add_highest(self.cards, pattern)
        return [0, [e[1] for e in pattern], pattern]

    def _add_highest(self, cards, pattern):
        card = None
        for each in cards:
            if each not in pattern and (card is None or (each[1]+12) % 13 > (card[1]+12) % 13):
                card = each
        pattern.append(card)
        return (card[1]+12) % 13+1

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
        print(' '.join([interpreter.interpret(c) for c in self.hand[2]]))
