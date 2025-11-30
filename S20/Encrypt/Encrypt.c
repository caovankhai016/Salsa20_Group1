ENCRYPT.C
#define _CRT_SECURE_NO_WARNINGS

#ifdef _WIN32
#include <windows.h>
#include <bcrypt.h>
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#pragma comment(lib, "bcrypt.lib")
#else
#include <fcntl.h>
#include <unistd.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "salsa20.h"

#define NIST_BLOCKS 2000

    int main()
{
    uint8_t key[32];
    uint8_t nonce[8];
    char plaintext[1024];
    uint8_t ciphertext[1024];

    const char *msg_file = "message.bin";
    const char *nist_file = "data.txt";

    printf("=== MA HOA SALSA20 (SECURE MODE) ===\n");

    printf("Nhap plaintext can ma hoa: ");
    if (fgets(plaintext, sizeof(plaintext), stdin) == NULL)
        return 0;

    size_t len = strlen(plaintext);
    if (len > 0 && plaintext[len - 1] == '\n')
        plaintext[--len] = '\0';

    printf("\n[1] Dang sinh khoa ngau nhien an toan...\n");

#ifdef _WIN32

    NTSTATUS status;
    status = BCryptGenRandom(NULL, key, 32, BCRYPT_USE_SYSTEM_PREFERRED_RNG);
    if (!NT_SUCCESS(status))
    {
        printf("LOI: Khong the tao Key ngau nhien (Windows BCrypt)!\n");
        return 1;
    }
    status = BCryptGenRandom(NULL, nonce, 8, BCRYPT_USE_SYSTEM_PREFERRED_RNG);
    if (!NT_SUCCESS(status))
    {
        printf("LOI: Khong the tao Nonce ngau nhien (Windows BCrypt)!\n");
        return 1;
    }
#else

    int fd = open("/dev/urandom", O_RDONLY);
    if (fd == -1)
    {
        perror("LOI: Khong the mo /dev/urandom");
        return 1;
    }
    if (read(fd, key, 32) != 32)
    {
        printf("LOI: Khong doc du 32 byte cho Key.\n");
        close(fd);
        return 1;
    }
    if (read(fd, nonce, 8) != 8)
    {
        printf("LOI: Khong doc du 8 byte cho Nonce.\n");
        close(fd);
        return 1;
    }
    close(fd);
#endif

    print_hex("Key:   ", key, 32);
    print_hex("Nonce: ", nonce, 8);

    memcpy(ciphertext, plaintext, len);

    salsa20_crypt(key, 32, nonce, 0, ciphertext, len);

    FILE *fp_msg = fopen(msg_file, "wb");
    if (fp_msg)
    {
        fwrite(key, 1, 32, fp_msg);
        fwrite(nonce, 1, 8, fp_msg);
        fwrite(ciphertext, 1, len, fp_msg);
        fclose(fp_msg);
        printf("Da xong file tin nhan: %s\n", msg_file);
    }
    else
    {
        printf("Khong the tao file %s\n", msg_file);
    }

    FILE *fp_nist = fopen(nist_file, "w");
    if (!fp_nist)
    {
        printf("Khong the tao file %s\n", nist_file);
        return 1;
    }

    uint8_t keystream_block[64];

    for (uint64_t counter = 0; counter < NIST_BLOCKS; counter++)
    {
        memset(keystream_block, 0, 64);

        salsa20_crypt(key, 32, nonce, counter, keystream_block, 64);

        for (int i = 0; i < 64; i++)
        {
            uint8_t byte = keystream_block[i];
            for (int bit = 7; bit >= 0; bit--)
            {
                int val = (byte >> bit) & 1;
                fputc(val ? '1' : '0', fp_nist);
            }
        }
    }
    fclose(fp_nist);
    printf("File '%s' chua %d bit lien tuc (san sang cho NIST STS).\n", nist_file, NIST_BLOCKS * 64 * 8);
    return 0;
