#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
/* calc/calc.h から必要なものをもってくる */
typedef int64_t LONG;
typedef uint64_t ULONG;
typedef struct monomial {
  LONG td;
  ULONG exp[1];
} *Monomial;
typedef struct ring {
 int nv;
 char **vname;
 int bpe; // byte per an exponent
 int wpe; // number of words per an exponent vector
 int graded; // set to 1 if the order is graded 
 int rev; // set to 1 if the order is revlex
 int (*mcomp)(Monomial,Monomial);
 LONG chr;
 ULONG sb; // sign bits
 /* Coef を使うものをここでは除いておく */
} *Ring;

Ring CurrentRing;

int mcomp_simple(Monomial a,Monomial b)
{
  int i,w,ret;

  if (CurrentRing->graded) {
    if (a->td>b->td) return 1;
    else if (a->td<b->td) return -1;
  }
  w=CurrentRing->wpe;
  ret=CurrentRing->rev?-1:1;
  for ( i=0; i<w; i++ )
    if (a->exp[i]>b->exp[i]) return ret;
    else if (a->exp[i]<b->exp[i]) return -ret;
  return 0;
}

int main() {
  Monomial f;
  Monomial g;
  int n=3;
  CurrentRing = (Ring) malloc(sizeof(struct ring));
  CurrentRing->graded=1;
  CurrentRing->wpe=n; /* n変数 */
  CurrentRing->rev=0; /* reverse lex order でない */
  f = (Monomial) malloc(sizeof(struct monomial)+sizeof(ULONG)*(n-1));
  g = (Monomial) malloc(sizeof(struct monomial)+sizeof(ULONG)*(n-1));
  f->exp[0]=2; f->exp[1]=1; f->exp[2]=1; f->td=4;
  g->exp[0]=2; g->exp[1]=2; g->exp[2]=0; g->td=4;
  printf("%d\n",mcomp_simple(f,g));
  printf("%d\n",mcomp_simple(g,f));
  printf("%d\n",mcomp_simple(f,f));
}
