#include <stdio.h>
#include <stdlib.h>
#include "obj0-lib.h"

struct object * new_object() {
  struct object * op;
  op = (struct object *)malloc(sizeof(struct object));
  op->tag=NIL;
  return op;
}

struct object * new_object_mint(int x) {
  struct object * op;
  op = new_object();
  op->tag=NUMBER;
  (op->body).mint = x;
  return op;
}

struct object * new_object_list(struct list *lst) {
  struct object *op;
  op = new_object();
  op->tag = LIST;
  (op->body).list = lst;
  return op;
}

struct list * new_list(struct object *op) {
  struct list * lst;
  lst = (struct list *)malloc(sizeof(struct list));
  lst->left = op;
  lst->right = NULL;
  return lst;
}

struct list * append(struct list *lst,struct object *op) {
  struct list *top;
  if (lst == NULL) return new_list(op);
  top = lst;
  while (lst != NULL) {
    if (lst->right == NULL) {
      lst->right = new_list(op);
      return top;
    }
    lst = lst->right;
  }
}

struct list * concat_list(struct list *lst,struct list *lst2) {
  struct list *top;
  if (lst == NULL) return lst2;
  top = lst;
  while (lst != NULL) {
    if (lst->right == NULL) {
      lst->right = lst2;
      return top;
    }
    lst = lst->right;
  }
}

void print_list(struct list *lst) {
  printf("[");
  while (lst != NULL) {
    print_object(lst->left);
    lst = lst->right;
    if (lst != NULL) printf(",");
  }
  printf("]");
}

void print_object(struct object *op) {
  if (op == NULL) return ;
  switch (op->tag) {
  case NUMBER:
    printf("%d",(op->body).mint);
    break;
  case LIST:
    print_list((op->body).list);
    break;
  case ID:
    printf("%c",(op->body).mint);
    break;
  default:
    fprintf(stderr,"Unknown tag=%d\n",op->tag);
    break;
  }
}

int main_test() {
  struct object *op;
  struct object *op2;
  struct object *op3;
  struct list *lst;
  int i;
  op = new_object_mint(123);
  print_object(op); putchar('\n');
  lst=NULL;
  for (i=0; i<5; i++) {
    lst=append(lst,op);
  }
  op2=new_object_list(lst);
  print_object(op2); putchar('\n');

  lst=new_list(op2);
  lst=append(lst,new_object_mint(-1));
  lst=append(lst,op2);
  op3=new_object_list(lst);
  print_object(op3); putchar('\n');
  return 0;
}

/*
int main() {
  main_test();
}
*/
