#include "calc.h"
int main(int argc,char **argv)
{
  char buf[BUFSIZ];
  int ordid,bpe,chr;
  Poly p;
  printf("variables:");
  scanf("%[^\n]", buf);
  printf("ordid:");scanf("%d", &ordid);
  printf("bpe:");scanf("%d", &bpe);
  printf("chr:");scanf("%d", &chr);
  printf("%s",set_parameters(buf, ordid, bpe, chr));
  while ( 1 ) {
    scanf("%*c");
    printf("Input: ");
    if (scanf("%[^\n]", buf) != 1) {
       return 0;
    }
    printf("%s\n",yyparse_str(buf));
  }
}
