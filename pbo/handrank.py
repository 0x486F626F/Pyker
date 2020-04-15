import math

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

    def possible_hands(self, useable, unuseable, searchlist=None):
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

    def eval(self, sec, pub, decksize, searchlist=None):
        myhands = self.possible_hands(sec+pub, [], searchlist)
        myprobs = hands_probs(decksize, 5-len(pub), myhands)
        mykeys = sorted(myprobs.keys())
        ophands = self.possible_hands(pub, sec, searchlist)
        opprobs = hands_probs(decksize, 7-len(pub), ophands)
        opkeys = sorted(opprobs.keys())

        #for i in range(1, len(mykeys)):
        #    myprobs[mykeys[i]] += myprobs[mykeys[i-1]]

        prev = 1
        s = 0
        for key in mykeys:
            t = myprobs[key]
            myprobs[key] *= prev
            s += myprobs[key]
            prev *= (1-t)
        print(s)

        prev = 1
        s = 0
        for key in opkeys:
            t = opprobs[key]
            opprobs[key] *= prev
            s += opprobs[key]
            prev *= (1-t)
        print(s)

        for i in range(1, len(opkeys)):
            opprobs[opkeys[-i-1]] += opprobs[opkeys[-i]]

        oidx = 0
        win = 0
        lose = 0
        tie = 0

        for my in mykeys:
            while opkeys[oidx] < my and oidx < len(opkeys):
                oidx += 1
            if oidx == len(opkeys):
                win += myprobs[my]
                continue

            op = opkeys[oidx]
            if my == op:        # could tie
                if oidx + 1 < len(opkeys):
                    nextop = opprobs[opkeys[oidx+1]]
                    tie += myprobs[my] * (opprobs[op]-nextop)
                    win += myprobs[my] * nextop
                    lose += myprobs[my] * (1 - opprobs[op])
                continue
            win += myprobs[my] * opprobs[op]
            lose += myprobs[my] * (1 - opprobs[op])

        """
        for my in mykeys:
            for op in opkeys:
                if my < op:
                    win += myprobs[my] * opprobs[op]
                if my == op:
                    tie += myprobs[my] * opprobs[op]
                if my > op:
                    lose += myprobs[my] * opprobs[op]

        """


        return [win, tie, lose, win+tie+lose]
                    

def hands_probs(decksize, todeal, hands):
    t = math.comb(decksize, todeal)
    probs = {}
    for each in hands:
        p = math.comb(decksize-each[2], todeal-each[2])/t
        if each[0] not in probs:
            probs[each[0]] = 0
        probs[each[0]] += p
    return probs

