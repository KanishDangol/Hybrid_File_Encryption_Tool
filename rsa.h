#ifndef RSA_H
#define RSA_H

#include <gmp.h>
#include <stddef.h>


void generate_rsa_keys(mpz_t n, mpz_t e, mpz_t d, unsigned int bits);
void rsa_encrypt_key(mpz_t ciphertext, const unsigned char *aes_key, size_t key_len, const mpz_t e, const mpz_t n);
void rsa_decrypt_key(unsigned char *recovered_key, size_t key_len, const mpz_t ciphertext, const mpz_t d, const mpz_t n);

#endif
