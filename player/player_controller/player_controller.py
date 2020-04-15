from abc import ABC, abstractmethod
from typing import List

from deck import Card
from trace import Trace


# Betting this number of chips is will make the character fold.
FOLD = -1


class PlayerController(ABC):
    """An interface for objects that control the behaviour of a player."""

    @abstractmethod
    def bet(self, hand: List[Card], bet_range: List[int], trace: Trace) -> int:
        """Returns the amount of chips that the player bets."""
