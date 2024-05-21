n = int(input())
mask = [[0 for i in range(n)] for i in range(n)]
for i in range(n):
    t = input().split()
    for j in range(n):
        mask[i][j] = int(t[j])

x, y = map(int, input().split())
d = int(input())

cur = [[0 for i in range(n)] for i in range(n)]
cur[x][y] = 1

def caninf(x, y):
    if x < 0 or y < 0 or x >= n or y >= n:
        return 0
    return cur[x][y] == 1 and not mask[x][y]

def calcinf(x, y):
    return caninf(x, y - 1) + caninf(x, y + 1) + caninf(x - 1, y) + caninf(x + 1, y)

poss = []

poss.append((x, y))

for day in range(d):
    pot = []
    for (x, y) in poss:
        for (i, j) in [(x, y - 1), (x, y + 1), (x - 1, y), (x + 1, y)]:
            if i < 0 or j < 0 or i >= n or j >= n:
                continue
            if cur[i][j] == 1:
                continue
            if not mask[i][j] and calcinf(i, j) >= 1:
                pot.append((i, j))
            elif mask[i][j] and calcinf(i, j) >= 4:
                pot.append((i, j))
    if len(pot) == 0:
        break
    poss = []
    for (x, y) in pot:
        if cur[x][y] == 1:
            continue
        cur[x][y] = 1
        poss.append((x, y))
#print(cur)
print(sum([sum(cur[i]) for i in range(n)]))
