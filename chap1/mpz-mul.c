/* gcc mpz-mul.c -lgmp */
#include <stdio.h>
#include <gmp.h>

void main() {
  mpz_t a;
  mpz_t b;
  int i;
  mpz_init(a); mpz_init(b);
  
  mpz_set_str(a,"2",10);
  mpz_set_str(b,"2",10);
  for (i=1; i<=1024; i++) {
    printf("2^%d= ",i);
    mpz_out_str(stdout,10,b);
    printf("\n");
    mpz_mul(b,a,b);
  }
}

