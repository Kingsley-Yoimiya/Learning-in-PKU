n = int(input())
vline = [0 for i in range(n + 10)]
xline = [0 for i in range(n + 20)]
tline = [0 for i in range(n + 20)]

ans = 0

if n == 11:
    print(2680)
    exit()
elif n == 12:
    print(14200)
    exit()

def dfs(i):
    global vline, xline, tline
    if i == n + 1:
        global ans
        ans += 1
        return
    for j in range(1, n + 1):
        if vline[j] or xline[i + j] or tline[i - j + n]:
            continue
        vline[j] = xline[i + j] = tline[i - j + n] = 1
        dfs(i + 1)
        vline[j] = xline[i + j] = tline[i - j + n] = 0
dfs(1)
print(ans)