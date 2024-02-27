n, m = map(int, input().split())
t = []
for i in range(n):
    t.append(input())
vst = [[0 for j in range (m)] for i in range(n)]
from queue import Queue
q = Queue()

for i in range(n):
    for j in range(m):
        if t[i][j] == 'S':
            q.put((i, j))
            vst[i][j] = 1

while not q.empty():
    x, y = q.get()
    for dx, dy in [(0, 1), (0, -1), (1, 0), (-1, 0)]:
        tx, ty = x + dx, y + dy
        if t[tx][ty] == '#':
            continue
        while t[tx + dx][ty + dy] != '#':
            tx, ty = tx + dx, ty + dy
        
        if t[tx][ty] == 'E':
            print(vst[x][y])
            exit()
        if t[tx][ty] == '.' and not vst[tx][ty]:
            vst[tx][ty] = vst[x][y] + 1
            q.put((tx, ty))
print(-1)