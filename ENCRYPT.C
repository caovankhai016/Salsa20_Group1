ENCRYPT.C
#define _CRT_SECURE_NO_WARNINGS

// === PH?N THÊM M?I: HEADER CHO SECURE RANDOM ===
#ifdef _WIN32
#include <windows.h>
#include <bcrypt.h>
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#pragma comment(lib, "bcrypt.lib") // T? ??ng link th? vi?n trên Visual Studio
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

    // Tên file ??u ra
    const char* msg_file = "message.bin"; // File ch?a tin nh?n mã hóa (?? gi?i mã)
    const char* nist_file = "data.txt";   // File ch?a 1 tri?u bit keystream

    printf("=== MA HOA SALSA20 (SECURE MODE) ===\n");

    // Nh?p plaintext
    printf("Nhap plaintext can ma hoa: ");
    if (fgets(plaintext, sizeof(plaintext), stdin) == NULL) return 0;

    // X? lý xóa xu?ng dòng
    size_t len = strlen(plaintext);
    if (len > 0 && plaintext[len - 1] == '\n') plaintext[--len] = '\0';



