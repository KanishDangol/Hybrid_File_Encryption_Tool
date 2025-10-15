#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include "rsa.h"
#include "hybrid.h"

void print_usage(const char *prog) {
    printf("Usage:\n");
    printf("  %s encrypt <input_file> <output_file>\n", prog);
    printf("  %s decrypt <input_file> <output_file>\n", prog);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        print_usage(argv[0]);
        return 1;
    }

    const char *mode = argv[1];
    const char *input_file = argv[2];
    const char *output_file = argv[3];

    mpz_t e, d, n;
    mpz_inits(e, d, n, NULL);
    mpz_set_ui(e, 1);  
    mpz_set_ui(d, 1); 
    mpz_set_ui(n, 1);  

    if (strcmp(mode, "encrypt") == 0) {
        hybrid_encrypt(input_file, output_file, e, n);
        printf("File encrypted successfully.\n");

    } else if (strcmp(mode, "decrypt") == 0) {
        hybrid_decrypt(input_file, output_file, d, n);
        printf("File decrypted successfully.\n");

    } else {
        print_usage(argv[0]);
        mpz_clears(e, d, n, NULL);
        return 1;
    }

    mpz_clears(e, d, n, NULL);
    return 0;
}
