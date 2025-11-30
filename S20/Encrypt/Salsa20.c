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
