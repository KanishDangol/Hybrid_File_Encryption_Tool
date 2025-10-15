#ifndef AES_H
#define AES_H

#include <stdint.h>

#define AES_256_KEY_SIZE 32
#define AES_256_EXPANDED_KEY_SIZE 240

void aes256_encrypt(uint8_t block[4][4], uint8_t key[32]);
void aes256_decrypt(uint8_t block[4][4], uint8_t key[32]);

#endif
