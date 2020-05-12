from polynom2 import Polynom2

# implement extended Euclid algorithm
def extended_euclid(a, b):
    if a == 0: return 0, 1, b
    if b == 0: return 1, 0, a

    x, y, d = extended_euclid(b, a % b)     # b * x  +  (a % b) * y == d
    return y, x - y * (a // b), d           # a % b = a - (a // b) * b   ==> a * y + b * (x - y * (a // b)) = d


# calculate a^-1 mod n if it exists
def invert_mod(a, n):
    x, y, d = extended_euclid(a, n)
    if d != 1:
        return None
    return x % n


# reflect bits from 0 to w-1
def reflect(w, x):
    res = 0
    for i in range(w // 2):
        v1 = (x >> i) & 1
        v2 = (x >> (w - i - 1)) & 1
        res |= (v1 << (w - i - 1))
        res |= (v2 << i)
    return res


def hack_crc32q(requested):
    poly = Polynom2(0x1814141AB) 
    x_n_inv = invert_mod(Polynom2(1 << 32), poly)
    return (requested * x_n_inv) % poly


def hack_crc32(requested):
    poly = Polynom2(0x104C11DB7)
    requested_norm = Polynom2(reflect(32, requested ^ 0xffffffff))
    x_n_inv = invert_mod(Polynom2(1 << 32), poly)

    rest = int((requested_norm * x_n_inv) % poly)
    rest_refl = sum(reflect(8, (rest >> sh) & 0xff) << sh for sh in (0, 8, 16, 24))
    return rest_refl ^ 0xffffffff


print("crc-32q: 0x%x" % hack_crc32q(0xF6ADBF28))
print("crc-32:  0x%x" % hack_crc32(0xFFAB723A))
