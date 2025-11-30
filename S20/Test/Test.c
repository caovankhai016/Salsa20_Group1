#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "salsa20.h"

#define MAX_LINE 512
#define MAX_HEX_SIZE 2048 // Tăng kích thước buffer để chứa đủ stream dài

int hex2int(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

int hex_string_to_bytes(const char* hex_str, uint8_t* out_bytes, int max_len) {
    int count = 0, val, high_nibble = -1;
    while (*hex_str && count < max_len) {
        val = hex2int(*hex_str);
        if (val >= 0) {
            if (high_nibble == -1) high_nibble = val;
            else {
                out_bytes[count++] = (high_nibble << 4) | val;
                high_nibble = -1;
            }
        }
        hex_str++;
    }
    return count;
}

void run_automated_tests(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("[ERROR] Khong tim thay file: %s (Kiem tra Working Directory!)\n", filename);
        return;
    }
    printf(">>> DANG KIEM TRA FILE: %s <<<\n", filename);

    char line[MAX_LINE];
    char key_hex[MAX_HEX_SIZE] = "";
    char iv_hex[MAX_HEX_SIZE] = "";
    char stream_hex[MAX_HEX_SIZE] = "";

    // 0: Nothing, 1: Key, 2: IV, 3: Stream
    int reading_mode = 0;
    int vector_count = 0, pass_count = 0;

    while (fgets(line, sizeof(line), fp)) {
        // Kiểm tra các từ khóa để chuyển chế độ đọc
        if (strstr(line, "key =")) {
            // Nếu đã gom đủ dữ liệu của vector trước đó, chạy test
            if (strlen(key_hex) > 0) {
                uint8_t key[32] = { 0 }, iv[8] = { 0 }, expected[64] = { 0 }, output[64] = { 0 };

                // Parse dữ liệu Hex
                int key_len = hex_string_to_bytes(key_hex, key, 32);
                hex_string_to_bytes(iv_hex, iv, 8);
                hex_string_to_bytes(stream_hex, expected, 64);

                // Reset Output về 0 trước khi encrypt
                memset(output, 0, 64);

                // Chạy thuật toán
                salsa20_crypt(key, key_len, iv, 0, output, 64);

                vector_count++;
                if (memcmp(output, expected, 64) == 0) {
                    pass_count++;
                }
                else {
                    printf("Vector #%d sai!\n", vector_count);
                }

                // Xóa buffer cũ
                key_hex[0] = '\0'; iv_hex[0] = '\0'; stream_hex[0] = '\0';
            }
            reading_mode = 1; // Bắt đầu đọc Key
        }
        else if (strstr(line, "IV =")) reading_mode = 2;
        else if (strstr(line, "stream[0..63] =")) reading_mode = 3;
        else if (strstr(line, "stream[")) reading_mode = 0; // Bỏ qua các phần stream sau
        else if (strstr(line, "xor-digest")) reading_mode = 0;
        else if (strstr(line, "Set")) reading_mode = 0; // Reset khi gặp Set mới

        if (reading_mode != 0) {
            char* start_ptr = line;
            char* eq_ptr = strchr(line, '=');

            if (eq_ptr) {
                // Nếu dòng có dấu '=', lấy phần sau dấu '='
                start_ptr = eq_ptr + 1;
            }
            // Nếu không có dấu '=' lấy toàn bộ dòng

            // Nối vào buffer tương ứng
            if (reading_mode == 1) strncat(key_hex, start_ptr, MAX_HEX_SIZE - strlen(key_hex) - 1);
            else if (reading_mode == 2) strncat(iv_hex, start_ptr, MAX_HEX_SIZE - strlen(iv_hex) - 1);
            else if (reading_mode == 3) strncat(stream_hex, start_ptr, MAX_HEX_SIZE - strlen(stream_hex) - 1);
        }
    }

    // Xử lý vector cuối cùng
    if (strlen(key_hex) > 0) {
        uint8_t key[32] = { 0 }, iv[8] = { 0 }, expected[64] = { 0 }, output[64] = { 0 };
        int key_len = hex_string_to_bytes(key_hex, key, 32);
        hex_string_to_bytes(iv_hex, iv, 8);
        hex_string_to_bytes(stream_hex, expected, 64);

        memset(output, 0, 64);
        salsa20_crypt(key, key_len, iv, 0, output, 64);

        vector_count++;
        if (memcmp(output, expected, 64) == 0) pass_count++;
        else printf("Vector cuoi cung sai!\n");
    }

    fclose(fp);
    printf("KET QUA: %d/%d vectors PASS.\n\n", pass_count, vector_count);
}

int main() {
    run_automated_tests("test_vector128.txt");
    run_automated_tests("test_vector256.txt");
    printf("An Enter de thoat...");
    getchar();
    return 0;
}