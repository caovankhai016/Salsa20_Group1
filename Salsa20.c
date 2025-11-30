void salsa20_crypt(uint8_t* key, int key_len, uint8_t* nonce,
    uint64_t initial_counter, uint8_t* buffer, size_t length)
{
    uint32_t input[16];
    uint32_t keystream_words[16];
    uint8_t keystream_bytes[64];
    uint64_t block_counter = initial_counter;
    size_t i;

    const char* constants = (key_len == 32) ? "expand 32-byte k" : "expand 16-byte k";
    const uint8_t* k = key;
    const uint8_t* n = nonce;
    const uint8_t* c = (const uint8_t*)constants;

    input[0] = U8TO32_LITTLE(c + 0);
    input[1] = U8TO32_LITTLE(k + 0);
    input[2] = U8TO32_LITTLE(k + 4);
    input[3] = U8TO32_LITTLE(k + 8);
    input[4] = U8TO32_LITTLE(k + 12);
    input[5] = U8TO32_LITTLE(c + 4);
    input[6] = U8TO32_LITTLE(n + 0);
    input[7] = U8TO32_LITTLE(n + 4);
    input[10] = U8TO32_LITTLE(c + 8);

    input[11] = U8TO32_LITTLE(k + (key_len == 32 ? 16 : 0));
    input[12] = U8TO32_LITTLE(k + (key_len == 32 ? 20 : 4));
    input[13] = U8TO32_LITTLE(k + (key_len == 32 ? 24 : 8));
    input[14] = U8TO32_LITTLE(k + (key_len == 32 ? 28 : 12));
    input[15] = U8TO32_LITTLE(c + 12);
    for (size_t offset = 0; offset < length; offset += 64) {
            input[8] = (uint32_t)(block_counter & 0xffffffff);
            input[9] = (uint32_t)(block_counter >> 32);

            salsa20_block(keystream_words, input);
            for (i = 0; i < 16; ++i) U32TO8_LITTLE(keystream_bytes + i * 4, keystream_words[i]);

            size_t bytes_to_xor = (length - offset < 64) ? (length - offset) : 64;
            for (i = 0; i < bytes_to_xor; ++i) buffer[offset + i] ^= keystream_bytes[i];
            block_counter++;
        }
    }
void print_hex(const char* label, uint8_t* data, size_t len) {
    printf("%s", label);
    for (size_t i = 0; i < len; i++) printf("%02X ", data[i]);
    printf("\n");
}
