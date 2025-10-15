#include <stdio.h>
#include <gmp.h>
#include "rsa.h"

int main() {
    mpz_t e, d, n;
    mpz_inits(e, d, n, NULL);

    generate_rsa_keys(e, d, n, 2048);

    FILE *pub = fopen("public.key", "w");
    FILE *priv = fopen("private.key", "w");

    gmp_fprintf(pub, "%Zx\n%Zx\n", e, n);
    gmp_fprintf(priv, "%Zx\n%Zx\n", d, n);

    fclose(pub);
    fclose(priv);

    mpz_clears(e, d, n, NULL);
    printf("Keys generated: public.key, private.key\n");
    return 0;
}
