#pragma once
#include <assert.h>

struct CRC {
    // Standard Ross N. Williams CRC parameters
    const char* name;
    const int width;
    const unsigned long poly;
    const unsigned long init;
    const unsigned long xor_out;
    const unsigned long check;
    const bool reflected;

    CRC(const char* n, int w, unsigned long p, unsigned long i, bool r, unsigned long x, unsigned long c) 
        : name(n), width(w), poly(p), init(i), xor_out(x), reflected(r), check(c) 
    {
        assert(8 <= width);  // implementation wouldn't work correctly for lesser widths
        assert(width <= sizeof(unsigned long) * 8);

        mask = (width == sizeof(unsigned long) * 8) ? (unsigned long)-1 : ((1ul << width) - 1);
        calc_table();        
    }

    struct Calc{
        Calc(const CRC& p) : parent(p) {
            reg = parent.init;
        }

        unsigned long update(const char* data, int len) { 
            reg = parent.update(reg, data, len);
            return reg;
        }

        unsigned long finish() const {
            return parent.finish(reg);
        }

        const CRC& parent;
        unsigned long reg;
    };

    Calc make_calc() const {
        return Calc(*this);
    }

    unsigned long calc(const char* data, int len) const {
        return finish(update(init, data, len));
    }

    unsigned long update(unsigned long reg, const char* data, int len) const;
    unsigned long finish(unsigned long reg) const;

    bool test() const {
        return check == calc("123456789", 9);
    }

//private:

    void calc_table();
    unsigned long table[256];
    unsigned long mask; 
};


