import deck
import math
import random
from pbo.handrank import Handrank
interpreter = deck.Deck()

class Player(object): 
    def __init__(self, balance):
        self.balance = balance
        self.inpool = 0
        self.card = []
        self.folded = False
        self.h = Handrank()

    def set_config(self, amp, bet, sr, ir):
        self.exp_amp = random.gauss(amp, math.sqrt(amp)) 
        self.bet_amp = random.gauss(bet, math.sqrt(bet))
        self.start_rate = random.gauss(sr, math.sqrt(sr))
        self.inc_rate = random.gauss(ir, math.sqrt(ir))

    def get_config(self):
        return [self.exp_amp, self.bet_amp, self.start_rate, self.inc_rate]

    def blind(self, blind):
        self.inpool += blind
        return blind

    def deal(self, card):
        self.card.append(card)

    def action(self, bet_range, trace):
        print('AI: Thinking...')
        e = self.h.eval(self.card[:2], self.card[2:], 52-len(self.card)) 
        limit = e[0] * self.bet_amp * self.balance - self.inpool
        print('== ', e[0], e[2], self.balance, self.inpool, limit)
        if e[0]*self.exp_amp < e[2]:
            print('AI: Fold')
            return -1


        r = min(1, self.start_rate + self.inc_rate * trace.stage_num)
        op = int(limit * random.uniform(r, 1)+0.5)
        op = min(op, bet_range[1])
        op = max(op, bet_range[0])
        self.inpool += op
        print('AI: +', op)
        return op


    def set_new_balance(self, b):
        self.balance = b

    def reset(self, ordering):
        self.folded = False
        self.card = []
        self.inpool = 0

    def notify_shownhands(self, hands):
        None

    def _print_cards(self):
        print(' '.join([interpreter.interpret(c) for c in self.card]))
