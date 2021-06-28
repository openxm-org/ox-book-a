#include <stdio.h>
#include <ctype.h>
#include "obj0-lib.h"

#define BSIZE 1024
#define NUMBER 1
char Ch;
char Sy;
int Value;
char Input[BSIZE];
int getch();
void in_symbol();
void expression();
struct object *list();
struct object *number();
struct object *expr();

struct object *push_obj(struct object *op);
struct object *pop_obj();

int main() {
  struct object *op;
  fgets(Input,BSIZE,stdin);
  Ch=' '; Sy=0; Value=0;
  op=expr();
  print_object(op); putchar('\n');
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
    Sy=Ch;
    Ch=getch();
  }
}


struct object *list() {
  struct object *op;
  struct list *lst=NULL;
  in_symbol();
  if (Sy==']') {
    op=new_object_list(NULL);
    in_symbol();
    return op;
  }
  aaa:
  if (Sy == NUMBER) {
    lst = append(lst,new_object_mint(Value));
    in_symbol();
  }else if (Sy == '[') {
    lst = append(lst,list());
  }
  if (Sy == ',') {
    in_symbol();
    goto aaa;
  }else if (Sy == ']') {
    in_symbol();
    return new_object_list(lst);
  }
  return NULL;
}
struct object *number() {
  struct object *op;
  op = new_object_mint(Value);
  in_symbol();
  return op;
} 

struct object * expr() {
  struct object *op;
  while (Sy != EOF) {
    in_symbol();
    if (Sy==NUMBER) {
      op=number();
    } else if (Sy=='[') {
      op=list();
    } else {
      printf("Sy=%d\n",Sy);
    }
    if (Sy==';') return op;
    else {
      fprintf(stderr,"Error: input is 0x%x [ %c ]. ; is expected\n",Sy,Sy);
      return NULL;
    }
  }
  fprintf(stderr,"Error: unexpected EOF\n");
  return NULL;
}
