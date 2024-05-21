# LCM
#import math as m
a, b = map(int, input().split(','))
def gcd(x, y):
    if x < y:
        return gcd(y, x)
    if y == 0:
        return x
    return gcd(y, x % y)
print (a * b // gcd(a, b))