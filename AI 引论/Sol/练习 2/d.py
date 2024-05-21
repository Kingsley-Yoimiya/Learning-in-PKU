val = [1, 4, 5, 9, 10, 40, 50, 90, 100, 400, 500, 900, 1000]
rom = ["I", "IV", "V", "IX", "X", "XL", "L", "XC", "C", "CD", "D", "CM", "M"]

def to_roman(n):
    for i in range(12, -1, -1):
        if val[i] <= n:
            return rom[i] + to_roman(n - val[i])
    return ""

def from_roman(s):
    if len(s) == 0:
        return 0
    for i in range(12, -1, -1):
        if rom[i] == s[:len(rom[i])]:
            return val[i] + from_roman(s[len(rom[i]):])
        
x = input()
if x[0] in "IVXLCDM":
    print(from_roman(x))
else:
    print(to_roman(int(x)))
