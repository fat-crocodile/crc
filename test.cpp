#include <stdio.h>
#include <cstring>
#include "crclib.h"

using namespace std;


int main(int argc, const char* argv[]) {    
    unsigned long ff = 0xfffffffful;
    unsigned long ffff = 0xfffffffffffffffful;

    // from https://crccalc.com/ and wikipedia
    //   name          width  poly       init ref   xorout  check
    CRC algos[] = {
        {"crc-32",        32, 0x04c11db7, ff, true,  ff, 0xCBF43926},
        {"crc-32/bzip2",  32, 0x04c11db7, ff, false, ff, 0xfc891918},
        {"crc-32c",       32, 0x1edc6f41, ff, true,  ff, 0xe3069283},
        {"crc-32d",       32, 0xa833982b, ff, true,  ff, 0x87315576},
        {"crc-32/mpeg-2", 32, 0x04c11db7, ff, false, 0,  0x0376e6e7},
        {"crc-32/posix",  32, 0x04c11db7, 0,  false, ff, 0x765e7680},
        {"crc-32q",       32, 0x814141ab, 0,  false, 0,  0x3010bf7f},
        {"crc-32/jamcrc", 32, 0x04c11db7, ff, true,  0,  0x340bc6d9},
        {"crc-32/xfer",   32, 0xaf,       0,  false, 0,  0xbd0be338}, 

        {"crc-16/ccitt-false", 16, 0x1021, 0xffff,  false, 0,  0x29b1},
        {"crc-16/arc",         16, 0x8005, 0,       true,  0,  0xbb3d},
        {"crc-16/arc-ccitt",   16, 0x1021, 0x1d0f,  false, 0,  0xe5cc},
        
        {"crc-8",          8, 0x07, 0,    false,  0,    0xf4},
        {"crc-8/cdma2000", 8, 0x9b, 0xff, false,  0,    0xda},
        {"crc-8/darc",     8, 0x39, 0,    true,   0,    0x15},
        {"crc-8/dvb-s2",   8, 0xd5, 0,    false,  0,    0xbc},
        {"crc-8/ebu",      8, 0x1d, 0xff, true,   0,    0x97},
        {"crc-8/i-code",   8, 0x1d, 0xfd, false,  0,    0x7e},
        {"crc-8/itu",      8, 0x07, 0,    false,  0x55, 0xa1},

        {"crc-10",          10, 0x233,  0,     false,  0,    0x199},
        {"crc-10/cdma2000", 10, 0x3d9,  0x3ff, false,  0,    0x233},
        {"crc-11",          11, 0x385,  0x1a,  false,  0,    0x5a3},
        {"crc-12/cdma2000", 12, 0xf13,  0xfff, false,  0,    0xd4d},
        {"crc-12/dect",     12, 0x80f,  0,     false,  0,    0xf5b},
        {"crc-13/bbc",      13, 0x1cf5, 0,     false,  0,    0x4fa},
        {"crc-14/darc",     14, 0x805,  0,     true,   0,    0x82d},
        {"crc-15",          15, 0x4599, 0,     false,  0,    0x59e},
        {"crc-15/mpt1327",  15, 0x6815, 0,     false,  0x1,  0x2566},

        {"crc-24",        24, 0x864cfb,   0xb704ce,  false, 0, 0x21cf02},

        {"crc-40/gsm",    40, 0x4820009,  0,         false, 0xfffffffffful, 0xd4164fc646},

        {"crc-64",        64, 0x42F0E1EBA9EA3693ul, 0,    false, 0,    0x6C40DF5F0B497347ul},
        {"crc-64/we",     64, 0x42F0E1EBA9EA3693ul, ffff, false, ffff, 0x62EC59E3F1A4F00Aul},
        {"crc-64/xz",     64, 0x42F0E1EBA9EA3693ul, ffff, true,  ffff, 0x995DC9BBDF1939FAul},
    };

    if (argc == 2 && strcmp("--test", argv[1]) == 0) {
        for (auto& a : algos) {
            printf("%s: %s\n", a.name, a.test() ? "ok" : "error" );
            if (!a.test()) {
                printf("    %lx != %lx\n", a.check, a.calc("123456789", 9));
            }
        } 
    }
    else {
        auto calc = algos[0].make_calc();  // crc-32 the same as python zlib.crc32
        
        for (;;) {
            char buffer[1024];
            size_t len = fread(buffer, 1, sizeof(buffer), stdin);

            if (len == 0) 
                break;

            calc.update(buffer, len);
        }

        printf("%x\n", calc.finish());
    } 
}

