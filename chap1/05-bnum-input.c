#include <stdio.h>
#include <string.h>
#define N 100
int str2bnum(char s[],int a[]) {
  int count;
  int i,j;
  count = strlen(s);
  if (s[count-1]=='\n') {
    s[count-1]=0;
    count--;
  }
  a[0] = count;
  for (i=count-1,j=1; i>=0;i--,j++) {
    a[j] = s[i]-'0';
  }
  return count;
}

void printbnum(int u[]) {
  int i;
  for (i=u[0]; i>0; i--) printf("%01d",u[i]);
}


int main() {
  char in[N+2];
  int a[N+1];
  fgets(in,N+2,stdin);
  str2bnum(in,a);
  printbnum(a);
  printf("\n");
}

