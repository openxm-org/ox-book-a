#include "fnot.h"

FILE *Input;
Ring CurrentRing;

Node get_vars()
{
  struct node root;
  Node p,p1;
  int c,i;
  char buf[BUFSIZ];
  char *s;

  p = &root;
  while ( (c = getc(Input)) != '[' );
  while ( 1 ) {
    c = skipspace();
    if ( c == ']' ) {
      p->next = 0;
      return root.next;
    } else {
      buf[0] = c;
      for ( i = 1; ; i++ ) {
        if ( i == BUFSIZ )
          error("get_vars : variable name too long");
        c = getc(Input);
        if ( !isalnum(c) ) {
          ungetc(c,Input);
          buf[i] = 0;
          break;
        } else
          buf[i] = c;
      }
      s = (char *)GC_malloc(i+1);
      strcpy(s,buf,i);
      NEWNODE(p1); p1->body = (void *)s;
      p->next = p1; p = p1;
    }
  }
}

// file format for a ring definition:
// chr ordid bpe
// [x y z ...]

int main(int argc,char **argv)
{
  Node vars;

  if ( argc == 1 ) Input = stdin;
  else Input = fopen(argv[1],"r"); 
  fscanf(Input,"%d %d %d\n",&chr,&ordid,&bpe);
  vars = get_vars();
  CurrentRing = create_ring(vars,ordid,bpe,chr);
  while ( 1 ) {
    yyparse();
  }
}
