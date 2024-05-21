a, b = input().split()
if len(a) < len(b):
    c = a
    a = b
    b = c
print(a + b)