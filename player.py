import deck
interpreter = deck.Deck()

class Player(object): 
    def __init__(self, balance):
        self.balance = balance
        self.card = []
        self.folded = False

    def small_blind(self, blind):
        self.balance -= blind

    def big_blind(self, blind):
        self.balance -= blind

    def deal(self, card):
        self.card.append(card)

    def action(self, bet_range = None, trace = None):
        self._print_cards()

        op = int(input())
        if op < 0:
            folded = True
            return op
        else:
            #TODO assert
            self.balance -= op
            return op

    def reset(self):
        self.folded = False
        self.card = []

    def _print_cards(self):
        print(' '.join([interpreter.interpret(c) for c in self.card]))
