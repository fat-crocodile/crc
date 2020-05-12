# polynom with modulo 2 coeffecients

class Polynom2(int):
    def __new__(cls, v):
        return  int.__new__(cls, v)
    def __add__(self, i):
        return Polynom2(self ^ i)
    def __sub__(self, i):
        return Polynom2(self ^ i)
    def __mul__(self, i):
        return Polynom2(_poly_mul(self, i))
    def __divmod__(self, i):
        res, rest = _poly_div(self, i)
        return Polynom2(res), Polynom2(rest)         
    def __floordiv__(self, i):
        res, _ = _poly_div(self, i)
        return Polynom2(res)
    def __mod__(self, i):
        _, rest = _poly_div(self, i)
        return Polynom2(rest)

    def __radd__(self, i):
        return self.__add__(i)
    def __rsub__(self, i):
        return self.__add__(i)
    def __rmul__(self, i):
        return self.__mul__(i)
    def __rdivmod__(self, i):
        res, rest = poly_div(i, self)
        return Polynom2(res), Polynom2(rest)         
    def __rfloordiv__(self, i):
        res, _ = _poly_div(i, self)
        return Polynom2(res)
    def __rmod__(self, i):
        _, rest = _poly_div(i, self)
        return Polynom2(rest)


def _poly_mul(a, b):
    res, d = 0, 0
    while b > 0:
        if b & 1 == 1:
            res ^= a << d
        d += 1
        b >>= 1
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

