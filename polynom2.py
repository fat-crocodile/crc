# polynom with modulo 2 coeffecients
# and euclid algorithm


def euclid(a, b):
    if a == 0: return b
    if b == 0: return a
    return euclid(b, a % b) 


def extended_euclid(a, b):
    if a == 0: return 0, 1, b
    if b == 0: return 1, 0, a

    x, y, d = extended_euclid(b, a % b)     # b * x  +  (a % b) * y == d
    return y, x - y * (a // b), d           # a % b = a - (a // b) * b   ==> a * y + b * (x - y * (a // b)) = d


class Polynom2(int):
    def __new__(cls, v):
        return  int.__new__(cls, v)
    def __add__(self, i):
        return Polynom(self ^ i)
    def __sub__(self, i):
        return Polynom(self ^ i)
    def __mul__(self, i):
        return Polynom(_poly_mul(self, i))
    def __divmod__(self, i):
        res, rest = _poly_div(self, i)
        return Polynom(res), Polynom(rest)         
    def __floordiv__(self, i):
        res, _ = _poly_div(self, i)
        return Polynom(res)
    def __mod__(self, i):
        _, rest = _poly_div(self, i)
        return Polynom(rest)

    def __radd__(self, i):
        return self.__add__(i)
    def __rsub__(self, i):
        return self.__add__(i)
    def __rmul__(self, i):
        return self.__mul__(i)
    def __rdivmod__(self, i):
        res, rest = poly_div(i, self)
        return Polynom(res), Polynom(rest)         
    def __rfloordiv__(self, i):
        res, _ = _poly_div(i, self)
        return Polynom(res)
    def __rmod__(self, i):
        _, rest = _poly_div(i, self)
        return Polynom(rest)


def _poly_mul(a, b):
    res, d = 0, 0
    while i > 0:
        if i & 1 == 1:
            res ^= self << d
        d += 1
        i >>= 1
    return res

def _poly_div(a, b):
    res, rest = 0, a
    w = b.bit_length()
    pow_diff = rest.bit_length() - w
    while pow_diff >= 0:
        rest ^= (b << pow_diff) 
        res |= 1 << pow_diff
        pow_diff = rest.bit_length() - w
    return res, rest
