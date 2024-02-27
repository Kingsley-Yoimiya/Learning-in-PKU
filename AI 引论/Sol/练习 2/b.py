# GCD
#a, b = map(int, input().split(','))
def gcd(x, y):
    if x < y:
        return gcd(y, x)
    if y == 0:
        return x
    return gcd(y, x % y)
n = int(input())
for i in range(n) :
    x, y = map(int, input().split())
    print(gcd(x, y))
