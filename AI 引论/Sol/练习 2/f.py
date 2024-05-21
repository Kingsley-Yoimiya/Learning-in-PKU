win = [0] * 100020
lost = [0] * 100020

t = int(input())
for i in range(t):
    x, y = map(int, input().split())
    win[x] = 1
    lost[y] = 1

arr = []

for i in range(1, 100001):
    if win[i] and lost[i] == 0:
        arr.append(i)

print(", ".join(map(str, arr)))