#include<stdio.h>
int euclid(int x, int y);
int main(void){
  int a, b;
  printf("a="); scanf("%d",&a);
  printf("b="); scanf("%d",&b);
  printf("gcd(%d, %d)=%d\n", a, b, euclid(a, b));
  return 0;
}
int euclid(int x, int y){
  if( x % y == 0 ) return y;
  else return euclid(y, x % y);
}
