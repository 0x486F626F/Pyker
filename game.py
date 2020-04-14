import deck
import player
import hand

class Game(object):
    def __init__(self, blind, players):
        self.blind = blind
        self.players = players
        self.nplayer = len(players)
        self.deck = deck.Deck()
        self.trace = []
        self.pools = []
        self.folded = []
        self.balances = [p.balance for p in players]
        self.cards = []

    def round(self, rnd_num):
        print('Round %d: ' % rnd_num)
        rnd_pool = [0 for i in range(self.nplayer)]
        i = 0
        if rnd_num == 0:
            print('Player 0 Small Blind %d' % self.blind)
            print('Player 1 Big Blind %d' % (self.blind * 2))
            self.players[0].small_blind(self.blind)
            self.balances[0] -= self.blind
            rnd_pool[0] = self.blind
            self.players[1].big_blind(self.blind * 2)
            self.balances[1] -= self.blind * 2
            rnd_pool[1] = self.blind * 2
            for i in range(self.nplayer):
                self.players[i].reset()
                card = self.deck.deal()
                self.cards[i].append(card)
                self.players[i].deal(card)
            for i in range(self.nplayer):
                card = self.deck.deal()
                self.cards[i].append(card)
                self.players[i].deal(card)

            i = 2
        elif rnd_num == 1:
            burned = self.deck.deal()
            cards = [self.deck.deal() for i in range(3)]
            for p in self.players:
                for c in cards:
                    p.deal(c)
            for each in self.cards:
                each += cards

        else:
            burned = self.deck.deal()
            card = self.deck.deal()
            for p in self.players:
                p.deal(card)
            for each in self.cards:
                each.append(card)

        non_raise = 0
        while non_raise < self.nplayer and len(self.pools[-1][1]) > 1:
            idx = i % self.nplayer
            i += 1
            if self.folded[idx] or self.balances[idx] == 0:
                non_raise += 1
                continue

            print('Player %d:' % idx)
            low = min(max(rnd_pool) - rnd_pool[idx], self.balances[idx])
            #TODO: tighter bound, support sidepools
            high = max(max(rnd_pool) - rnd_pool[idx], self.balances[idx])

            if high == 0:
                continue

            op = 0
            while True:
                op = self.players[idx].action([low, high])
                if op < 0 or (op >= low and op <= high):
                    break

            if op > low:       # raise
                non_raise = 1
                rnd_pool[idx] += op
                self.balances[idx] -= op
            elif op == low:    # check
                non_raise += 1
                rnd_pool[idx] += op
                self.balances[idx] -= op
            elif op > 0 and self.balances[idx] - op == 0: #all-in
                non_raise += 1
                rnd_pool[idx] += op
                self.balances[idx] -= op
            else:
                self.folded[idx] = True
                non_raise += 1
                self.pools[-1][1].remove(idx)

        self.pools[-1][0] += sum(rnd_pool)
        
        if rnd_num < 3:
            self.round(rnd_num + 1)

    def show(self, pool):
        if len(pool[1]) == 1:
            self.balances[pool[1][0]] += pool[0]
        else:
            candidates = [[i, hand.Hand(self.cards[i])] for i in pool[1]]
            candidates = sorted(candidates, key=lambda x: (x[1], x[0]))
            winners = []
            for c in candidates:
                print(c[0])
                c[1].printhand()
                if c[1] == candidates[-1][1]:
                    winners.append(c[0])
            print(pool[0], winners)
            for i in winners:
                self.balances[i] += pool[0] // len(winners)
            self.balances[winners[0]] += pool[0] % len(winners)
            print(self.balances)


    def newgame(self):
        self.pools = [[0, [i for i in range(self.nplayer)]]]
        self.trace = []
        self.cards = [[] for i in range(self.nplayer)]
        self.folded = [False for i in range(self.nplayer)]
        self.round(0)

        self.show(self.pools[0])
        self.rotate()

    def rotate(self):
        self.balances = self.balances[1:] + [self.balances[0]]
        self.players = self.players[1:] + [self.players[0]]
  

