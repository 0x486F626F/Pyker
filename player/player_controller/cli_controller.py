from typing import List

from deck import Card
from trace import Trace
from .player_controller import PlayerController


class CLIController(PlayerController):
    """Lets a human type in their actions."""

    def bet(self, _: List[Card], bet_range: List[int], trace: Trace) -> int:
        # TODO improve
        bet = int(input())
        return bet
