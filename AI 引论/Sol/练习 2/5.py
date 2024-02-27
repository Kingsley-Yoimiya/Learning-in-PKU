n, k = map(int, input().split())
k = (1 + k / 100)

fl = 0
cur = 0
val = 200

for i in range(1, 21):
    cur += n
    if cur >= val:
        fl = 1
        print(i)
        break
    val = val * k
if not fl:
    print("Impossible")
