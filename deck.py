import random


suit_base = [127137, 127153, 127169, 127185]


class Deck(object):
    def __init__(self):
        self.deck = [[i, j] for i in range(4) for j in range(13)]
        random.shuffle(self.deck)
        self.index = 0

    def deal(self):
        if self.index >= 52:
            return None
        self.index += 1
        return self.deck[self.index-1]

    def interpret(self, card):
        if card[1] < 11:
            return chr(suit_base[card[0]] + card[1])
        return chr(suit_base[card[0]] + card[1] + 1)
