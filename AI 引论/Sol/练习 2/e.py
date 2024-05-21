m = int(input())
t1 = m / 0.4883
t1 = min(t1, 240.0)
m -= t1 * 0.4883
t2 = m / 0.5383
t2 = min(t2, 160.0)
m -= t2 * 0.5383
t3 = m / 0.7883
print(f"{t1 + t2 + t3 :.2f}")