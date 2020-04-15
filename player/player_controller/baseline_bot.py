from random import random
from typing import List

from deck import Card
from hand import Hand
from trace import Trace
from .player_controller import PlayerController, FOLD


class BaselineBot(PlayerController):
    """A simple bot that bets based on the strength of its hand."""

    def bet(self, hand_cards: List[Card], bet_range: List[int], trace: Trace) -> int:
        try:
            # bet depending on the strength of the hand
            hand = Hand(hand_cards)
            hand_rank = hand.hand[0]
            bet = hand_rank * 10
            # fold if the other players have bet too much, fold with 50% chance
            if bet < bet_range[0] and random() < 0.5:
                bet = FOLD
        except TypeError:
            bet = 0

        # clamp bet to range
        if bet != FOLD:
            bet = min(max(bet, bet_range[0]), bet_range[1])

        return bet
