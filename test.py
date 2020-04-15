import game
from player import Player
from baseline_bot import BaselineBot


p1 = Player(100)
p2 = BaselineBot(75)
p3 = BaselineBot(50)
g = game.Game(1, [100, 75, 50], [p1, p2, p3])

g.start()
