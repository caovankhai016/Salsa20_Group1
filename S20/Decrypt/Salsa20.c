#define _CRT_SECURE_NO_WARNINGS
#include "salsa20.h"
#include <stdio.h>
#include <string.h>

#define ROTL(a,b) (((a) << (b)) | ((a) >> (32 - (b))))
#define QR(a, b, c, d) ( \
	b ^= ROTL(a + d, 7), \
	c ^= ROTL(b + a, 9), \
	d ^= ROTL(c + b, 13), \
	a ^= ROTL(d + c, 18))

static uint32_t U8TO32_LITTLE(const uint8_t* p) {
	return (uint32_t)p[0] | ((uint32_t)p[1] << 8) |
		((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

static void U32TO8_LITTLE(uint8_t* p, uint32_t v) {
	p[0] = (uint8_t)(v);       p[1] = (uint8_t)(v >> 8);
	p[2] = (uint8_t)(v >> 16); p[3] = (uint8_t)(v >> 24);
}
void salsa20_block(uint32_t out[16], uint32_t const in[16]) {
    int i;
    uint32_t x[16];
    for (i = 0; i < 16; ++i) x[i] = in[i];
    for (i = 0; i < 20; i += 2) {
        QR(x[0], x[4], x[8], x[12]); QR(x[5], x[9], x[13], x[1]);
        QR(x[10], x[14], x[2], x[6]); QR(x[15], x[3], x[7], x[11]);
        QR(x[0], x[1], x[2], x[3]); QR(x[5], x[6], x[7], x[4]);
        QR(x[10], x[11], x[8], x[9]); QR(x[15], x[12], x[13], x[14]);
    }
    for (i = 0; i < 16; ++i) out[i] = x[i] + in[i];
}

