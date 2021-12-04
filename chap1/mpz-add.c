/* gcc mpz-add.c -lgmp */
#include <stdio.h>
#include <gmp.h>

int main() {
  mpz_t a;
  mpz_t b;
  mpz_t c;
  mpz_init(a); mpz_init(b); mpz_init(c);
  
  mpz_set_str(a,"9998",10);
  mpz_set_str(b,"990002",10);
  mpz_add(c,a,b);
  mpz_out_str(stdout,10,c);
  printf("\n");
}

