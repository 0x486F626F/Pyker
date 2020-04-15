import handrank


h = handrank.Handrank()
p = h.possible_hands([[1, 10], [1, 9], [1, 11]], [[1, 0], [1, 12]])
print(len(p))
for i in range(5):
    print(p[i][0], h.decode(p[i][1]), p[i][2])
