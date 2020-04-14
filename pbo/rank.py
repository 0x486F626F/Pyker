import hand
import deck
import threading

deck = deck.Deck()

all_cards = [deck.deal() for i in range(52)]
all_hands = [[] for i in range(52)]
threads = []


def worker(a):
    for b in range(a+1, 52):
        for c in range(b+1, 52):
            for d in range(c+1, 52):
                for e in range(d+1, 52):
                    all_hands[a].append(hand.Hand([all_cards[a], all_cards[b], all_cards[c],
                        all_cards[d], all_cards[e]]))
        print('Thread %d: %s%s %f' % (a, '*'*b, '-'*(51-b), b/0.51))
    print('Thread %d joined' % a)
 

for a in range(52):
    threads.append(threading.Thread(target=worker, args=(a,)))

window = 13

for i in range(0, 52, window):
    for t in range(window):
        threads[i+t].start()
        print('Thread %d started' % (i+t))
    for t in range(window):
        threads[i+t].join()

hands = []
for h in all_hands:
    hands += h
hands.sort()
hands.reverse()

print(len(hands))
rank = 0
for i in range(10):
    hands[i].printhand()

prev = hands[0]
rank = 0
count = 0
f = open('handrank.txt', 'w')
for h in hands:
    if prev > h:
        rank = count
    f.write('%d %d ' % (rank, h.hand[0]))
    for c in h.hand[2]:
        f.write('%d %d ' % (c[0], c[1]))
    f.write('\n')
    prev = h
    count += 1
