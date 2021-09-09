#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct bignum {
  int len;
  int u[1];
};

struct bignum *str2bnum(char s[]) {
  int count;
  int i,j;
  struct bignum *a;
  count = strlen(s);
  a = (struct bignum *)malloc(sizeof(struct bignum)+sizeof(int)*count);
  a->len = count; //bnum-input.c と同じ形式にするため u[0]を無駄な領域に.
  for (i=count-1,j=1; i>=0;i--,j++) {
    a->u[j] = s[i]-'0';
  }
  return a;
}

void printbnum(struct bignum *bp) {
  int i;
  for (i=bp->len; i>0; i--) printf("%01d",bp->u[i]);
}

int main() {
  struct bignum *bp;
  bp = str2bnum("12345678901234567890");
  printbnum(bp); putchar('\n');
}




