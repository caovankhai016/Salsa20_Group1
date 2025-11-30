#ifndef SALSA20_H
#define SALSA20_H

#include <stdint.h>
#include <stddef.h>

void salsa20_block(uint32_t out[16], uint32_t const in[16]);
void salsa20_crypt(uint8_t* key, int key_len, uint8_t* nonce,
    uint64_t initial_counter, uint8_t* buffer, size_t length);
void print_hex(const char* label, uint8_t* data, size_t len);

#endif