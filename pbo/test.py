import handrank
import threading


h = handrank.Handrank()

result = [[[] for i in range(52)] for j in range(52)]


def worker(i,j):
    ci = [i//13,i%13]
    cj = [j//13,j%13]
    result[i][j] = result[j][i] = h.eval([ci,cj],[],50)
    print(ci, cj, result[i][j])

threads = []

for i in range(52):
    for j in range(i+1,52):
        threads.append(threading.Thread(target=worker, args=(i,j,)))

print(len(threads))
w = 2

for i in range(0, len(threads), w):
    for t in range(w):
        threads[i+t].start()
    for t in range(w):
        threads[i+t].join()
        print(i+t)

f = open('start.txt', 'w')

for i in range(52):
    for j in range(i+1,52):
        r = result[i][j]
        f.write('%d %d %d %d %f %f %f %f\n' % (i//13,i%13,j//13,j%13, r[0], r[1], r[2], r[3]))
