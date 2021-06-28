#include <stdio.h>
#include <ctype.h>

#define BSIZE 1024
#define NUMBER 1
char Ch;
char Sy;
int Value;
char Input[BSIZE];
int getch();
void in_symbol();
void expression();

int main() {
  fgets(Input,BSIZE,stdin);
  Ch=' '; Sy=0; Value=0;
  expression();
  return 0;
}

int getch() {
  static int pt=0;
  if (Input[pt] == 0) return -1;
  return Input[pt++];
}

void in_symbol() {
  int sign;
  while (isspace(Ch)) Ch=getch();
  if (isdigit(Ch) || (Ch=='-')) {
    Sy=NUMBER;
    Value=0;
    if (Ch=='-') {
      sign=-1; Ch=getch();
    }else sign=1;
    do {
      Value = Value*10+(Ch-'0');
      Ch=getch();
    }while (isdigit(Ch));
    Value = Value*sign;
  }else if (Ch <0) {
    Sy=EOF;
    return ;
  }else {
    fprintf(stderr,"Error: %c is not a part of a number\n",Ch);
    Sy=Ch;
    Ch=getch();
  }
}

void expression() {
  while (Sy != EOF) {
    in_symbol();
    if (Sy==NUMBER) printf("Value=%d\n",Value);
    else {
      printf("Sy=%d\n",Sy);
    }
  }
}
