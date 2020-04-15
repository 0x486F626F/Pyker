import game
import pboai

conf = [1.5, 1.5, 0.8, 0.05]
p1 = pboai.Player(100)
p2 = pboai.Player(100)

f = open('conf.txt', 'w')

for i in range(100):
    p1.set_new_balance(100)
    p1.set_config(conf[0], conf[1], conf[2], conf[3], i+1)
    p2.set_new_balance(100)
    p2.set_config(conf[0], conf[1], conf[2], conf[3], i+1)
    g = game.Game(1, [100, 100], [p1, p2])

    conf = g.start()
    print(conf)
    for e in conf:
        f.write('%f ' % e)
    f.write('\n')
