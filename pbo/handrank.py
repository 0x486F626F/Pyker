
class Handrank(object):
    def __init__(self):
        self.load()

    def load(self):
        f = open('handrank.txt', 'r')
        self.ranks = []
        for line in f:
            line = line.split()
            rank = int(line[0])
            cards = [[int(line[i*2+2]), int(line[i*2+3])] for i in range(5)]
            self.ranks.append([rank, self.encode(cards)])
        print("loaded")

    def encode(self, hand):
        code = 0
        for c in hand:
            code += 2 ** (c[0] * 13 + c[1])
        return code

    def decode(self, code):
        cards = []
        for i in range(52):
            if ((code >> i) & 1) > 0:
                cards.append([i//13, i%13])
        return cards

    def hascard(self, card, hand):
        n = 2 ** (card[0] * 13 + card[1])
        return (n & hand) > 0

    def possible_hands(self, useable, unuseable, searchlist = None):
        if searchlist is None:
            searchlist = self.ranks
        w = self.encode(useable)
        wo = self.encode(unuseable)
        result = []
        for each in searchlist:
            overlap = each[1] & w
            missing = 5 - len(self.decode(overlap))
            if each[1] & wo == 0 and missing <= 7-len(useable):
                result.append(each + [missing])
        return result

