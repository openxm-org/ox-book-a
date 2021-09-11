/* for myplot */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "obj0-lib.h"
#include "glib4.h"

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

struct list *Xpts;
struct list *Ypts;
struct object *read_command(FILE *fp);
int main() {
  struct object *op;
  struct list *lst;
  glib_open();
  printf("Input a list of the form (-10<x,y<10), e.g., [[-9,0,5,9],[5,0,2,5]];\n");
  printf("[Enter] shows an example.\n> ");
  op=read_command(stdin);
  if (op == NULL) return -1;
  /* check format */
  if (op->tag != LIST) {
    fprintf(stderr,"Error: Input a list\n"); return -1;
  }
  lst=(op->body).list;
  if (lst == NULL) {
    fprintf(stderr,"Error: list format\n"); return -1;
  }
  /* 以下 format の確認を省略. 点のリスト */
  Xpts=((lst->left)->body).list;
  lst=lst->right;
  Ypts=((lst->left)->body).list; 

  glib_loop();
  return 0;
}


void glib_draw() {
  struct list *xx;
  struct list *yy;
  int x,y;
  glib_window(-10,-10,10,10);
  glib_color(NULL,0xff0000);
  glib_line(-10,0,10,0); glib_line(0,-10,0,10);
  xx = Xpts; yy = Ypts;
  glib_color(NULL,0x00ffff);
  while ((xx != NULL) && (yy != NULL)) {
    /* no check of format */
    glib_set_point(x=(xx->left->body).mint,y=(yy->left->body).mint);
    printf("x, y=%d, %d\n",x,y);
    xx = xx->right; yy = yy->right;
  }
}

struct object *read_command(FILE *fp) {
  struct object *op;
  fgets(Input,BSIZE,fp);
  if (strlen(Input) < 2) strcpy(Input,"[[-9,0,5,9],[5,0,2,5]];\n"); 
  Ch=' '; Sy=0; Value=0;
  op=expr();
  print_object(op); putchar('\n');
  return op;
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
