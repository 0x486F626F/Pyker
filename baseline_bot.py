from trace import Trace
from typing import List, Tuple

import deck
from hand import Hand


Card = Tuple[int, int]

# Betting this number of chips is will make the character call.
CALL: int = 0
# Betting this number of chips is will make the character fold.
FOLD: int = -1

interpreter = deck.Deck()


class BaselineBot:
    balance: int
    hand: List[Card]
    folded: bool

    def __init__(self, initial_balance: int):
        self.balance = initial_balance
        self.folded = False
        self.hand = []

    def blind(self, blind: int):
        self.balance -= blind
        return blind

    def deal(self, card: Card):
        self.hand.append(card)

    # TODO make bet_range a Tuple[int, int] or a class
    def action(self, bet_range: List[int], trace: Trace) -> int:
        """
        Lets the character bet, check/call or raise.
        Returns either the amount of chips that is added to the pot or FOLD, if the character folded.
        """

        print("(bot) ", end="")
        self._print_cards()
        trace.debug()

        try:
            # bet depending on the strength of the hand
            hand = Hand(self.hand)
            hand_rank = hand.hand[0]
            bet = hand_rank * 10
            # fold if the other players have bet too much
            if bet < bet_range[0]:
                bet = FOLD
        except TypeError:
            bet = CALL

        # clamp bet to range
        if bet not in [FOLD, CALL]:
            bet = min(bet, bet_range[1])

        if bet == FOLD:
            self.folded = True
        else:
            if bet == CALL:
                bet = bet_range[0]
            self.balance -= bet

        print("bet:", bet)

        return bet

    def set_new_balance(self, balance):
        self.balance = balance

    def reset(self, ordering: int):
        """Prepares the character for the start of a new round."""
        self.folded = False
        self.hand = []

    def notify_shownhands(self, hands):
        pass

    def _print_cards(self):
        print(' '.join([interpreter.interpret(c) for c in self.hand]))
