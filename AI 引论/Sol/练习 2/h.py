n, m = map(int, input().split())
h = [0 for i in range(n * m)]
mx = [0 for i in range(n * m)]
v = [(0, -1), (0, 1), (1, 0), (-1, 0)]
ans = 0

def dfs(x, y):
    if mx[x * m + y] > 0:
        return mx[x * m + y]
    for (tx, ty) in v:
        tx += x
        ty += y
        if tx < 0 or ty < 0 or tx >= n or ty >= m:
            continue
        if h[tx * m + ty] < h[x * m + y]:
            mx[x * m + y] = max(mx[x * m + y], dfs(tx, ty))
    mx[x * m + y] += 1
    return mx[x * m + y]


for i in range(n):
    t = input().split()
    for j in range(m):
        h[i * m + j] = int(t[j])

for i in range(n):
    for j in range(m):
        ans = max(ans, dfs(i, j))

print(ans)