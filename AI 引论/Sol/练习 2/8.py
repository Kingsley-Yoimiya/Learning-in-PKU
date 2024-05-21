a = input()
b = input()
if a in b:
    print(f"{a} is substring of {b}")
elif b in a:
    print(f"{b} is substring of {a}")
else:
    print("No substring")