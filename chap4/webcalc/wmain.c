#include "calc.h"
int main(int argc,char **argv)
{
  char buf[BUFSIZ],*ring_info,*result_str;
  int ordid,bpe,chr;
  printf("variables:");
  scanf("%[^\n]", buf);
  printf("ordid:");scanf("%d", &ordid);
  printf("bpe:");scanf("%d", &bpe);
  printf("chr:");scanf("%d", &chr);
  ring_info = set_parameters(buf, ordid, bpe, chr);
  printf("%s\n",ring_info);
  FREE(ring_info);
  while ( 1 ) {
    scanf("%*c");
    printf("Input: ");
    if (scanf("%[^\n]", buf) != 1) {
       return 0;
    }
    result_str = yyparse_str(buf);
    printf("%s\n",result_str);
    FREE(result_str);
  }
}
