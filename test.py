import game
import player
import pboai

p1 = pboai.Player(100)
p1.set_config(1.2, 1, 0.5, 0.1)
p2 = pboai.Player(100)
p2.set_config(2, 1.5, 0.6, 0.75)
g = game.Game(1, [100, 100], [p1, p2])

conf = g.start()
print(conf)
