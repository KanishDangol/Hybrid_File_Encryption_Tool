#ifndef AES_H
#define AES_H

#include <stdint.h>

// Function prototypes for encryption and decryption
void aes256_encrypt(uint8_t block[4][4], uint8_t key[32]);
void aes256_decrypt(uint8_t block[4][4], uint8_t key[32]);

#endif
