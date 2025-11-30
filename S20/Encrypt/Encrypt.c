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
// ===============================================

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "salsa20.h"

// 1 block = 64 byte = 512 bit.
// 2000 block = 1.024.000 bit
#define NIST_BLOCKS 2000 

int main() {
    uint8_t key[32];
    uint8_t nonce[8];
    char plaintext[1024];
    uint8_t ciphertext[1024];

    const char* msg_file = "message.bin";
    const char* nist_file = "data.txt";   

    printf("=== MA HOA SALSA20 (SECURE MODE) ===\n");

    printf("Nhap plaintext can ma hoa: ");
    if (fgets(plaintext, sizeof(plaintext), stdin) == NULL) return 0;


    size_t len = strlen(plaintext);
    if (len > 0 && plaintext[len - 1] == '\n') plaintext[--len] = '\0';



