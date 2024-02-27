v = input().split()
l = []
for i in range(6):
    l.append(int(v[i]))

even = [i for i in l if i % 2 == 0]
odd  = [i for i in l if i % 2 == 1]
print(abs(min(even) - max(odd)))