import game
import player

p1 = player.Player(100)
p2 = player.Player(75)
p3 = player.Player(50)
g = game.Game(1, [100, 75, 50], [p1, p2, p3])

g.start()
