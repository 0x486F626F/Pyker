import deck
interpreter = deck.Deck()

class Player(object): 
    def __init__(self, balance):
        self.balance = balance
        self.card = []
        self.folded = False

    def blind(self, blind):
        self.balance -= blind
        return blind

    def deal(self, card):
        self.card.append(card)

    def action(self, bet_range, trace):
        self._print_cards()
        trace.debug()

        op = int(input())
        if op < 0:
            folded = True
            return op
        else:
            #TODO assert
            self.balance -= op
            return op

    def set_new_balance(self, b):
        self.balance = b

    def reset(self, ordering):
        self.folded = False
        self.card = []

    def notify_shownhands(self, hands):
        None

    def _print_cards(self):
        print(' '.join([interpreter.interpret(c) for c in self.card]))
