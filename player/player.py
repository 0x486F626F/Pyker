from trace import Trace
from typing import List

from deck import Card
from .player_controller import PlayerController, FOLD


class Player:
    controller: PlayerController
    balance: int
    hand: List[Card]
    folded: bool

    def __init__(self, controller: PlayerController, initial_balance: int):
        self.controller = controller
        self.balance = initial_balance
        self.folded = False
        self.hand = []

    def blind(self, blind: int):
        self.balance -= blind
        return blind

    def deal(self, card: Card):
        self.hand.append(card)

    def action(self, bet_range: List[int], trace: Trace) -> int:
        """
        Lets the player bet, check/call or raise.
        Returns either the amount of chips that is added to the pot or FOLD, if the character folded.
        """

        self._print_cards()
        trace.debug()

        bet = self.controller.bet(self.hand, bet_range, trace)

        if bet == FOLD:
            self.folded = True
        else:
            # TODO make sure that the PlayerControllers don't do this
            assert(self.balance >= bet)
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
        print(' '.join([str(card) for card in self.hand]))
