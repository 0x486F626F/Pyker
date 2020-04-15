import game
from player import Player, BaselineBot, CLIController


p1 = Player(CLIController(), 100)
p2 = Player(BaselineBot(), 75)
p3 = Player(BaselineBot(), 50)
g = game.Game(1, [100, 75, 50], [p1, p2, p3])

g.start()
