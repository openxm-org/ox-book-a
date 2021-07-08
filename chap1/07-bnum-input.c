#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int *str2bnum(char s[]) {
  int count;
  int i,j;
  int *a;
  count = strlen(s);
  a = (int *)malloc(sizeof(int)*(count+1));
  a[0] = count;
  for (i=count-1,j=1; i>=0;i--,j++) {
    a[j] = s[i]-'0';
  }
  return a;
}

void printbnum(int u[]) {
  int i;
  for (i=u[0]; i>0; i--) printf("%01d",u[i]);
}


int main() {
  char *in;
  char *new;
  int size=10;
  int i,c;
  int *a;
  i=0; in = (char *)malloc(sizeof(char)*size);
  while ((c=getchar()) > '\n') {
    if (i < size-1) {
      in[i++]=c; in[i]=0;
    }else{
      size = 2*size;  //文字列の格納領域を２倍に
      new = (char *)malloc(sizeof(char)*size);
      // 新しい領域にコピー
      for (i=0; in[i] != 0; i++) {
	new[i]=in[i]; new[i+1]=0;
      }
      free(in);
      in = new; //2倍のものを新しいinに
      in[i++]=c; in[i]=0;
    }
  }
  a=str2bnum(in);
  printbnum(a);
  printf("\n");
}

