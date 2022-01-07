#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
/* calc/calc.h から必要なものをもってくる */
typedef int64_t LONG;
typedef uint64_t ULONG;

typedef union {
  LONG f;
/* GMP は使わないので除いておく. */
//  mpz_ptr z;
//  mpq_ptr q; 
} Coef;

typedef struct ring {
 int nv;
 char **vname;
 int bpe; // byte per an exponent
 int wpe; // number of words per an exponent vector
 int graded; // set to 1 if the order is graded 
 int rev; // set to 1 if the order is revlex
// int (*mcomp)(Monomial,Monomial);
 LONG chr;
 ULONG sb; // sign bits
 /* Coef を使うものをここでは除いておく */
} *Ring;

Ring CurrentRing;


Coef inv_ff(Coef a)
{
  LONG f1,f2,a1,a2,q,r,p;
  Coef u;

  p=CurrentRing->chr;
  f1=a.f; f2=p; a1=1; a2=0;
  while ( 1 ) {
    q=f1/f2; r=f1 - f2*q; 
    f1=f2; f2=r;
    if (f2==0) 
      break;
    r=(a2*q)%p; r=a1-r;
    if (r<0) r += p;
    a1=a2; a2=r;
  }
  u.f=a2;
  return u;
}

int main() {
  Coef x;
  LONG p=7;
  CurrentRing = (Ring) malloc(sizeof(struct ring));
  CurrentRing->chr=p; /* characteristic = p. mod p で計算. */
  printf("Input a number: ");
  /* LONG が long long と解釈される場合は %ld なく %lld */
  scanf("%lld",&(x.f));
  if (x.f % p == 0) {
    printf("Not invertible\n");
  }else{
    printf("%lld^(-1) mod %lld = %lld\n",x.f,p,inv_ff(x).f);
  }
}
