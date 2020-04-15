import game
import player
import pboai

p1 = player.Player(100)
p2 = pboai.Player(100)
g = game.Game(1, [100, 100], [p1, p2])

g.start()
