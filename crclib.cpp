#include "crclib.h"

namespace { 
    // utils for bit-magic
    unsigned long divide_byte_poly(int width, unsigned long poly, unsigned char data); 
    unsigned long reflect(int width, unsigned long x);
}


unsigned long CRC::update(unsigned long reg, const char* data, int len) const {
    if (!reflected) {
        for (int i = 0; i < len; ++i) {
            unsigned char hb = (reg >> (width - 8)) & 0xff;   // get last register byte
            reg <<= 8;                                        // shift register to the left
            reg ^= table[(unsigned char)(hb ^ data[i])];      // update register
        }
    } 
    else {
        for (int i = 0; i < len; ++i) {
            unsigned char hb = reg & 0xff;                    // get first register byte
            reg >>= 8;                                        // shift register to the right
            reg ^= table[(unsigned char)(hb ^ data[i])];      // update register
        }
    }
    return reg & mask;
}

unsigned long CRC::finish(unsigned long reg) const {
    return (reg ^ xor_out) & mask;        
}


void CRC::calc_table() {
    if (!reflected) {
        for (int i = 0; i < 256; ++i)
            table[i] = divide_byte_poly(width, poly, i) & mask;
    } 
    else {
        for (int i = 0; i < 256; ++i)
            table[i] = reflect(width, divide_byte_poly(width, poly, reflect(8, i)) & mask);
    }
}


namespace {
    // divide polynom like a0000 by poly
    unsigned long divide_byte_poly(int width, unsigned long poly, unsigned char data) {
        unsigned long res = 0;
        unsigned long high_bit_test = 1 << (width - 1);

        for (int i = 0; i < 8; ++i) {
            int high_bit = (res >> (width - 1)) & 1;
            res <<= 1;
            if (high_bit ^ ((data >> 7) & 1)) 
                res ^= poly;
            data <<= 1;
        }

        return res;
    }


    unsigned long set_bit(unsigned long value, int index, int bit) {
        if (bit)
            value |= (1UL << index);
        else
            value &= ~(1UL << index);
        return value;
    }


    unsigned long reflect(int from, int to, unsigned long x) {
        for (; from < to; ++from, --to) {
            unsigned long v1 = (x >> from) & 1;
            unsigned long v2 = (x >> to) & 1;
            x = set_bit(x, from, v2);
            x = set_bit(x, to, v1);
        } 
        return x;
    }

    unsigned long reflect(int width, unsigned long x) {
        return reflect(0, width - 1, x);
    }
}

