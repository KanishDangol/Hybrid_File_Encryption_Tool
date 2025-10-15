#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "aes.h"

// Utility functions
void bytes_to_block(const uint8_t *input, uint8_t block[4][4]) {
    for (int i = 0; i < 16; i++) {
        block[i % 4][i / 4] = input[i];
    }
}

void block_to_bytes(const uint8_t block[4][4], uint8_t *output) {
    for (int i = 0; i < 16; i++) {
        output[i] = block[i % 4][i / 4];
    }
}

void generate_random_aes_key(uint8_t *key, size_t len) {
    FILE *fp = fopen("/dev/urandom", "rb");
    if (!fp || fread(key, 1, len, fp) != len) {
        fprintf(stderr, "Error generating AES key.\n");
        exit(1);
    }
    fclose(fp);
}

void hybrid_encrypt(const char *input_file, const char *output_file, void *unused1, void *unused2) {
    FILE *in = fopen(input_file, "rb");
    FILE *out = fopen(output_file, "wb");
    if (!in || !out) {
        fprintf(stderr, "File error.\n");
        exit(1);
    }

    // Read original size
    fseek(in, 0, SEEK_END);
    size_t original_size = ftell(in);
    fseek(in, 0, SEEK_SET);
    fwrite(&original_size, sizeof(size_t), 1, out);

    // Generate AES key
    uint8_t aes_key[32];
    generate_random_aes_key(aes_key, sizeof(aes_key));

    // Save encrypted AES key to aes.key 
    FILE *keyfile = fopen("aes.key", "wb");
    if (!keyfile || fwrite(aes_key, 1, 32, keyfile) != 32) {
        fprintf(stderr, "Failed to write AES key file.\n");
        exit(1);
    }
    fclose(keyfile);

    // Print AES key 
    printf("Generated AES key:\n");
    for (int i = 0; i < 32; i++) printf("%02X", aes_key[i]);
    printf("\n");

    // Encrypt file in 16-byte blocks
    uint8_t buffer[16] = {0};
    size_t read;
    while ((read = fread(buffer, 1, 16, in)) > 0) {
        if (read < 16) memset(buffer + read, 0, 16 - read);

        uint8_t block[4][4];
        bytes_to_block(buffer, block);
        aes256_encrypt(block, aes_key);
        block_to_bytes(block, buffer);
        fwrite(buffer, 1, 16, out);
    }

    fclose(in);
    fclose(out);
}

void hybrid_decrypt(const char *input_file, const char *output_file, void *unused1, void *unused2) {
    FILE *in = fopen(input_file, "rb");
    FILE *out = fopen(output_file, "wb");
    if (!in || !out) {
        fprintf(stderr, "File error.\n");
        exit(1);
    }

    size_t original_size;
    fread(&original_size, sizeof(size_t), 1, in);

    // Load the RSA-decrypted AES key from aes.key
    uint8_t aes_key[32];
    FILE *keyfile = fopen("aes.key", "rb");
    if (!keyfile || fread(aes_key, 1, 32, keyfile) != 32) {
        fprintf(stderr, "Failed to read AES key.\n");
        exit(1);
    }
    fclose(keyfile);

    // Print AES key 
    printf("Recovered AES key:\n");
    for (int i = 0; i < 32; i++) printf("%02X", aes_key[i]);
    printf("\n");

    // Decrypt file
    uint8_t buffer[16];
    size_t total_written = 0;
    while (fread(buffer, 1, 16, in) == 16) {
        uint8_t block[4][4];
        bytes_to_block(buffer, block);
        aes256_decrypt(block, aes_key);
        block_to_bytes(block, buffer);

        size_t to_write = (original_size - total_written >= 16) ? 16 : (original_size - total_written);
        fwrite(buffer, 1, to_write, out);
        total_written += to_write;
    }

    fclose(in);
    fclose(out);
}
