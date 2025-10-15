#include "rsa.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void generate_rsa_keys(mpz_t n, mpz_t e, mpz_t d, unsigned int bits) {
    mpz_t p, q, phi, pp, qq;
    gmp_randstate_t state;

    mpz_inits(p, q, phi, pp, qq, NULL);
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, time(NULL));

    mpz_urandomb(p, state, bits / 2);
    mpz_nextprime(p, p);

    mpz_urandomb(q, state, bits / 2);
    mpz_nextprime(q, q);

    mpz_mul(n, p, q);
    mpz_sub_ui(pp, p, 1);
    mpz_sub_ui(qq, q, 1);
    mpz_mul(phi, pp, qq);
    mpz_set_ui(e, 65537);

    if (mpz_invert(d, e, phi) == 0) {
        fprintf(stderr, "Error: e and phi are not coprime.\n");
        exit(1);
    }

    mpz_clears(p, q, phi, pp, qq, NULL);
    gmp_randclear(state);
}

void rsa_encrypt_key(mpz_t ciphertext, const unsigned char *aes_key, size_t key_len, const mpz_t e, const mpz_t n) {
    mpz_t pt;
    mpz_init(pt);

    // Ensure AES key is always exactly 32 bytes and big-endian
    unsigned char padded_key[32] = {0};
    memcpy(padded_key + (32 - key_len), aes_key, key_len);
    mpz_import(pt, 32, 1, 1, 0, 0, padded_key);

    gmp_printf("Plaintext AES key as int: %Zd\n", pt);

    if (mpz_cmp(pt, n) >= 0) {
        fprintf(stderr, "Error: AES key too large for RSA modulus.\n");
        exit(1);
    }

    mpz_powm(ciphertext, pt, e, n);
    mpz_clear(pt);
}

void rsa_decrypt_key(unsigned char *recovered_key, size_t key_len,
                     const mpz_t ciphertext, const mpz_t d, const mpz_t n) {
    mpz_t decrypted;
    mpz_init(decrypted);
    mpz_powm(decrypted, ciphertext, d, n);

    gmp_printf("Decrypted AES key as int: %Zd\n", decrypted);

    unsigned char temp[512] = {0};  
    size_t count = 0;

    mpz_export(temp, &count, 1, 1, 0, 0, decrypted);

    memset(recovered_key, 0, key_len);
    if (count > key_len) {
        memcpy(recovered_key, temp + (count - key_len), key_len);
    } else {
        size_t padding = key_len - count;
        memmove(recovered_key + padding, temp, count);
        memset(recovered_key, 0, padding);
    }

    mpz_clear(decrypted);
}
