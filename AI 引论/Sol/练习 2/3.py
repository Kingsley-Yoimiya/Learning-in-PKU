n = int(input())
v = input().split()
t = []
for i in range(n):
    t.append(int(v[i]))
t.sort()

if n % 2 == 0:
    ret = (t[n // 2 - 1] + t[n // 2]) / 2
    if ret == int(ret):
        print(int(ret))
    else:
        print(f"{ret:.1f}")
else:
    print(f"{t[n // 2]}")