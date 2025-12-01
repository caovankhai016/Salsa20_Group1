
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "salsa20.h"

int main() {
    uint8_t key[32];
    uint8_t nonce[8];
    uint8_t buffer[1024];
    const char* filename = "message.bin"; // File chứa key, nonce và ciphertext
    size_t len;

    printf("=== GIAI MA ===\n");

    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        printf("Khong tim thay '%s'\n", filename);
        return 1;
    }

    if (fread(key, 1, 32, fp) != 32) { printf("Loi doc Key\n"); return 1; }
    if (fread(nonce, 1, 8, fp) != 8) { printf("Loi doc Nonce\n"); return 1; }
    len = fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);

    if (len == 0) { printf("File rong.\n"); return 1; }

    printf("\nNhan duoc %zu bytes ma hoa.\n", len);
    print_hex("Key: ", key, 32);
    salsa20_crypt(key, 32, nonce, 0, buffer, len);
    buffer[len] = '\0';
    printf("\nNOI DUNG GOC: %s\n\n", buffer);
    return 0;
}
