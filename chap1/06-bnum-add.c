#include <stdio.h>
// a+b->c
void bnum_add(int a[],int b[],int c[]) {
  int *t;
  int bb;
  int i;
  if (a[0] < b[0]) {t=a; a=b; b=t;}
  c[1] = 0;
  for (i=1; i<=a[0]; i++) {
    if (i <= b[0]) bb = b[i]; else bb=0;
    c[i] = c[i]+a[i]+bb;
    if (c[i] >= 10) {c[i+1]=1; c[i] = c[i]%10; c[0]=i+1;}
    else {c[i+1]=0; c[0]=i;}
  }
}
void print_bnum(int u[]) {
  int i;
  for (i=u[0]; i>0; i--) printf("%01d",u[i]);
}
void main() {
  int a[10]={4,8,9,9,9};
  int b[10]={6,2,0,0,0,9,9};
  int c[10];
  bnum_add(a,b,c); print_bnum(c); printf("\n");
}
