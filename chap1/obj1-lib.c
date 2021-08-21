#include <stdio.h>
#include "obj0-lib.h"
int eq_object(struct object *op1, struct object *op2);
int eq_list(struct list *lst1,struct list *lst2);
struct list *insert(struct list *root,struct object *op, struct object *newop);

int eq_list(struct list *lst1,struct list *lst2) {
  if (lst1 == lst2) return 1;
  if ((lst1 == NULL) || (lst2 == NULL)) return 0;
  if (eq_object(lst1->left,lst2->left)) {
    return eq_list(lst1->right,lst2->right);
  }else return 0;
}
int eq_object(struct object *op1, struct object *op2) {
  if (op1 == op2) return 1;
  if (op1->tag != op2->tag) return 0;
  switch(op1->tag) {
    case NUMBER:
      if ((op1->body).mint == (op2->body).mint) return 1;
      else return 0;
      break;
    case LIST:
      return eq_list((op1->body).list, (op2->body).list);
      break;
    default:
      fprintf(stderr,"eq_object は tag %d については実装されていません.\n",op1->tag);
  }
  return 0;
}
/*
  リスト root の object op の次に newop を挿入してリストを作り直す.
  op は リストの最上位レベルのみで探す. 
    たとえば [0,1,op,4] は op を見つけるが,
    [0,1,[op,3],4] では op を見つけない.
  object の比較は eq_object で行う.
*/
struct list *insert(struct list *root,struct object *op, struct object *newop) {
  struct list *next;
  struct list *new;
  if (root == NULL) return new_list(newop);
  next = root;
  while (next != NULL) {
    if (eq_object(op,next->left)) {
      new = new_list(newop);
      new->right=next->right;
      next->right=new;
      return root;
    }else{
      next=next->right;
    }
  }
  return NULL;  // not found.
}


/* test codes */
int main() {
  struct list *lst1;
  struct list *lst2;
  int i;
  lst1=new_list(new_object_mint(1));
  for (i=2; i<5; i++) lst1 = append(lst1,new_object_mint(i));
  print_list(lst1);  // [1,2,3,4] を生成.
  printf("\n");
  
  lst2=insert(lst1,new_object_mint(3),new_object_mint(30));
  if (lst2 != NULL) print_list(lst2);  // [1,2,3,30,4] のはず.
  printf("\n");
  
  return 0;
}
