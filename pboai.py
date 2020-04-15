import deck
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

    def blind(self, blind):
        self.inpool += blind
        return blind

    def deal(self, card):
        self.card.append(card)

    def action(self, bet_range, trace):
        print('AI: Thinking...')
        e = self.h.eval(self.card[:2], self.card[2:], 52-len(self.card)) 
        limit = e[0] * (self.balance - self.inpool)
        if limit < 0:
            print('AI: Fold')
            return -1

        op = int(limit * random.uniform(0.5+trace.stage_num*0.1, 1)+0.5)
        op = min(op, bet_range[1])
        op = max(op, bet_range[0])
        print('AI: +', op)


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
