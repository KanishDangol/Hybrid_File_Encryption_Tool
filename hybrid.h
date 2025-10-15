#ifndef HYBRID_H
#define HYBRID_H

void hybrid_encrypt(const char *input_file, const char *output_file, mpz_t e, mpz_t n);
void hybrid_decrypt(const char *input_file, const char *output_file, mpz_t d, mpz_t n);

#endif
