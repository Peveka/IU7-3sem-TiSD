from string import ascii_letters
import random
s = ""
alph = ascii_letters + '0123456789'
ln = int(input())
i = 0
while i != ln:
    c = alph[random.randint(0, len(alph)-1)]
    if c not in s:
        s += c
        i+=1

print(s)
