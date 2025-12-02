from string import ascii_letters
import random
s = ""
alph = ascii_letters + '0123456789'
for i in range(100):
    s+=alph[random.randint(0, len(alph)-1)]

print(s)
